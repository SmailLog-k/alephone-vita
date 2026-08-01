#include "vita_sound.h"
#include "vita_platform.h"
#include <cstdio>
#include <cstring>
#include <cmath>

VitaSoundSystem g_vita_sound;

VitaSoundSystem::VitaSoundSystem() 
    : device(nullptr), context(nullptr), mixer_running(false) {
    buffers.reserve(MAX_SOUND_BUFFERS);
    sources.reserve(MAX_SOUND_SOURCES);
    
    for (int i = 0; i < MAX_SOUND_SOURCES; i++) {
        sources.emplace_back();
    }
}

VitaSoundSystem::~VitaSoundSystem() {
    Shutdown();
}

bool VitaSoundSystem::Initialize() {
    std::lock_guard<std::mutex> lock(sound_mutex);
    
    // Open device
    device = alcOpenDevice(nullptr);
    if (!device) {
        printf("Failed to open OpenAL device\n");
        return false;
    }
    
    // Create context
    ALCint attributes[] = {
        ALC_FREQUENCY, 44100,
        ALC_REFRESH, 60,
        ALC_SYNC, ALC_TRUE,
        0
    };
    context = alcCreateContext(device, attributes);
    if (!context) {
        printf("Failed to create OpenAL context\n");
        alcCloseDevice(device);
        device = nullptr;
        return false;
    }
    
    if (!alcMakeContextCurrent(context)) {
        printf("Failed to make OpenAL context current\n");
        alcDestroyContext(context);
        alcCloseDevice(device);
        context = nullptr;
        device = nullptr;
        return false;
    }
    
    // Initialize sources
    for (int i = 0; i < MAX_SOUND_SOURCES; i++) {
        alGenSources(1, &sources[i].source);
        CheckOpenALError("alGenSources");
        sources[i].in_use = false;
        sources[i].volume = 1.0f;
        sources[i].pitch = 1.0f;
        sources[i].looping = false;
        sources[i].queue_head = 0;
        sources[i].queue_tail = 0;
        sources[i].queue_count = 0;
    }
    
    // Set default listener
    SetListenerPosition(0, 0, 0);
    SetListenerOrientation(0, 0, -1, 0, 1, 0);
    
    // Start mixer thread
    mixer_running = true;
    mixer_thread = std::thread(&VitaSoundSystem::MixerThread, this);
    
    printf("Vita Sound System initialized\n");
    return true;
}

void VitaSoundSystem::Shutdown() {
    mixer_running = false;
    if (mixer_thread.joinable()) {
        mixer_thread.join();
    }
    
    StopAllSounds();
    
    // Delete sources
    for (int i = 0; i < MAX_SOUND_SOURCES; i++) {
        if (sources[i].source) {
            alDeleteSources(1, &sources[i].source);
            sources[i].source = 0;
        }
    }
    
    // Delete buffers
    for (auto& buffer : buffers) {
        if (buffer.buffer) {
            alDeleteBuffers(1, &buffer.buffer);
            buffer.buffer = 0;
        }
    }
    buffers.clear();
    
    if (context) {
        alcMakeContextCurrent(nullptr);
        alcDestroyContext(context);
        context = nullptr;
    }
    
    if (device) {
        alcCloseDevice(device);
        device = nullptr;
    }
    
    printf("Vita Sound System shutdown\n");
}

void VitaSoundSystem::CheckOpenALError(const char* operation) {
    ALenum error = alGetError();
    if (error != AL_NO_ERROR) {
        const char* error_str = "Unknown";
        switch (error) {
            case AL_INVALID_NAME: error_str = "AL_INVALID_NAME"; break;
            case AL_INVALID_ENUM: error_str = "AL_INVALID_ENUM"; break;
            case AL_INVALID_VALUE: error_str = "AL_INVALID_VALUE"; break;
            case AL_INVALID_OPERATION: error_str = "AL_INVALID_OPERATION"; break;
            case AL_OUT_OF_MEMORY: error_str = "AL_OUT_OF_MEMORY"; break;
        }
        printf("OpenAL Error [%s]: %s (0x%X)\n", operation, error_str, error);
    }
}

int VitaSoundSystem::FindFreeSource() {
    for (int i = 0; i < MAX_SOUND_SOURCES; i++) {
        if (!sources[i].in_use) {
            ALint state;
            alGetSourcei(sources[i].source, AL_SOURCE_STATE, &state);
            if (state != AL_PLAYING && state != AL_PAUSED) {
                return i;
            }
        }
    }
    return -1;
}

int VitaSoundSystem::FindFreeBuffer() {
    for (int i = 0; i < (int)buffers.size(); i++) {
        if (!buffers[i].loaded) {
            return i;
        }
    }
    if ((int)buffers.size() < MAX_SOUND_BUFFERS) {
        buffers.emplace_back();
        return buffers.size() - 1;
    }
    return -1;
}

bool VitaSoundSystem::LoadWAVFile(const char* filename, VitaSoundBuffer& buffer) {
    FILE* fp = fopen(filename, "rb");
    if (!fp) {
        return false;
    }
    
    // Read WAV header
    struct WAVHeader {
        char riff[4];
        uint32_t chunk_size;
        char wave[4];
        char fmt[4];
        uint32_t fmt_size;
        uint16_t audio_format;
        uint16_t num_channels;
        uint32_t sample_rate;
        uint32_t byte_rate;
        uint16_t block_align;
        uint16_t bits_per_sample;
        char data[4];
        uint32_t data_size;
    } header;
    
    if (fread(&header, sizeof(header), 1, fp) != 1) {
        fclose(fp);
        return false;
    }
    
    // Validate
    if (strncmp(header.riff, "RIFF", 4) != 0 ||
        strncmp(header.wave, "WAVE", 4) != 0 ||
        strncmp(header.fmt, "fmt ", 4) != 0 ||
        header.audio_format != 1) {
        fclose(fp);
        return false;
    }
    
    // Read data
    std::vector<uint8_t> data(header.data_size);
    if (fread(data.data(), 1, header.data_size, fp) != header.data_size) {
        fclose(fp);
        return false;
    }
    fclose(fp);
    
    // Create OpenAL buffer
    alGenBuffers(1, &buffer.buffer);
    CheckOpenALError("alGenBuffers");
    
    ALenum format;
    if (header.num_channels == 1) {
        format = (header.bits_per_sample == 8) ? AL_FORMAT_MONO8 : AL_FORMAT_MONO16;
    } else {
        format = (header.bits_per_sample == 8) ? AL_FORMAT_STEREO8 : AL_FORMAT_STEREO16;
    }
    
    alBufferData(buffer.buffer, format, data.data(), data.size(), header.sample_rate);
    CheckOpenALError("alBufferData");
    
    buffer.loaded = true;
    buffer.filename = filename;
    buffer.channels = header.num_channels;
    buffer.sample_rate = header.sample_rate;
    buffer.bits = header.bits_per_sample;
    
    return true;
}

bool VitaSoundSystem::LoadOGGFile(const char* filename, VitaSoundBuffer& buffer) {
    // Simplified - in production use stb_vorbis or similar
    // For now, return false to fallback to WAV
    return false;
}

int VitaSoundSystem::LoadSound(const char* filename) {
    std::lock_guard<std::mutex> lock(sound_mutex);
    
    int buffer_id = FindFreeBuffer();
    if (buffer_id < 0) {
        printf("No free sound buffers\n");
        return -1;
    }
    
    if (LoadWAVFile(filename, buffers[buffer_id])) {
        return buffer_id;
    }
    
    // Try OGG
    if (LoadOGGFile(filename, buffers[buffer_id])) {
        return buffer_id;
    }
    
    buffers[buffer_id].loaded = false;
    return -1;
}

void VitaSoundSystem::UnloadSound(int buffer_id) {
    std::lock_guard<std::mutex> lock(sound_mutex);
    
    if (buffer_id >= 0 && buffer_id < (int)buffers.size() && buffers[buffer_id].loaded) {
        alDeleteBuffers(1, &buffers[buffer_id].buffer);
        CheckOpenALError("alDeleteBuffers");
        buffers[buffer_id].buffer = 0;
        buffers[buffer_id].loaded = false;
    }
}

int VitaSoundSystem::PlaySound(int buffer_id, float volume, float pitch, bool looping) {
    std::lock_guard<std::mutex> lock(sound_mutex);
    
    if (buffer_id < 0 || buffer_id >= (int)buffers.size() || !buffers[buffer_id].loaded) {
        return -1;
    }
    
    int source_id = FindFreeSource();
    if (source_id < 0) {
        return -1;
    }
    
    VitaSoundSource& source = sources[source_id];
    source.in_use = true;
    source.buffer = buffers[buffer_id].buffer;
    source.volume = volume * sfx_volume * master_volume;
    source.pitch = pitch;
    source.looping = looping;
    
    alSourcei(source.source, AL_BUFFER, source.buffer);
    CheckOpenALError("alSourcei buffer");
    
    alSourcef(source.source, AL_GAIN, source.volume);
    alSourcef(source.source, AL_PITCH, source.pitch);
    alSourcei(source.source, AL_LOOPING, looping ? AL_TRUE : AL_FALSE);
    alSourcei(source.source, AL_SOURCE_RELATIVE, AL_TRUE);
    CheckOpenALError("alSourcef");
    
    alSourcePlay(source.source);
    CheckOpenALError("alSourcePlay");
    
    return source_id;
}

int VitaSoundSystem::PlaySound3D(int buffer_id, float x, float y, float z, float volume, float pitch, bool looping) {
    int source_id = PlaySound(buffer_id, volume, pitch, looping);
    if (source_id >= 0) {
        SetSourcePosition(source_id, x, y, z);
        sources[source_id].in_use = true;
        alSourcei(sources[source_id].source, AL_SOURCE_RELATIVE, AL_FALSE);
        CheckOpenALError("alSourcei relative");
    }
    return source_id;
}

void VitaSoundSystem::StopSound(int source_id) {
    std::lock_guard<std::mutex> lock(sound_mutex);
    
    if (source_id >= 0 && source_id < MAX_SOUND_SOURCES && sources[source_id].in_use) {
        alSourceStop(sources[source_id].source);
        CheckOpenALError("alSourceStop");
        alSourcei(sources[source_id].source, AL_BUFFER, 0);
        CheckOpenALError("alSourcei clear buffer");
        sources[source_id].in_use = false;
        sources[source_id].buffer = 0;
    }
}

void VitaSoundSystem::StopAllSounds() {
    std::lock_guard<std::mutex> lock(sound_mutex);
    
    for (int i = 0; i < MAX_SOUND_SOURCES; i++) {
        if (sources[i].in_use) {
            alSourceStop(sources[i].source);
            alSourcei(sources[i].source, AL_BUFFER, 0);
            CheckOpenALError("alSourceStop all");
            sources[i].in_use = false;
            sources[i].buffer = 0;
        }
    }
}

void VitaSoundSystem::SetSourceVolume(int source_id, float volume) {
    if (source_id >= 0 && source_id < MAX_SOUND_SOURCES && sources[source_id].in_use) {
        sources[source_id].volume = volume * sfx_volume * master_volume;
        alSourcef(sources[source_id].source, AL_GAIN, sources[source_id].volume);
        CheckOpenALError("alSourcef gain");
    }
}

void VitaSoundSystem::SetSourcePitch(int source_id, float pitch) {
    if (source_id >= 0 && source_id < MAX_SOUND_SOURCES && sources[source_id].in_use) {
        sources[source_id].pitch = pitch;
        alSourcef(sources[source_id].source, AL_PITCH, pitch);
        CheckOpenALError("alSourcef pitch");
    }
}

void VitaSoundSystem::SetSourcePosition(int source_id, float x, float y, float z) {
    if (source_id >= 0 && source_id < MAX_SOUND_SOURCES && sources[source_id].in_use) {
        alSource3f(sources[source_id].source, AL_POSITION, x, y, z);
        CheckOpenALError("alSource3f position");
    }
}

void VitaSoundSystem::SetSourceVelocity(int source_id, float x, float y, float z) {
    if (source_id >= 0 && source_id < MAX_SOUND_SOURCES && sources[source_id].in_use) {
        alSource3f(sources[source_id].source, AL_VELOCITY, x, y, z);
        CheckOpenALError("alSource3f velocity");
    }
}

bool VitaSoundSystem::IsSourcePlaying(int source_id) {
    if (source_id < 0 || source_id >= MAX_SOUND_SOURCES || !sources[source_id].in_use) {
        return false;
    }
    
    ALint state;
    alGetSourcei(sources[source_id].source, AL_SOURCE_STATE, &state);
    return state == AL_PLAYING;
}

void VitaSoundSystem::SetListenerPosition(float x, float y, float z) {
    alListener3f(AL_POSITION, x, y, z);
    CheckOpenALError("alListener3f position");
}

void VitaSoundSystem::SetListenerVelocity(float x, float y, float z) {
    alListener3f(AL_VELOCITY, x, y, z);
    CheckOpenALError("alListener3f velocity");
}

void VitaSoundSystem::SetListenerOrientation(float at_x, float at_y, float at_z, float up_x, float up_y, float up_z) {
    float orientation[] = {at_x, at_y, at_z, up_x, up_y, up_z};
    alListenerfv(AL_ORIENTATION, orientation);
    CheckOpenALError("alListenerfv orientation");
}

void VitaSoundSystem::SetMasterVolume(float volume) {
    master_volume = std::max(0.0f, std::min(1.0f, volume));
    
    // Update all active sources
    for (int i = 0; i < MAX_SOUND_SOURCES; i++) {
        if (sources[i].in_use) {
            sources[i].volume = sources[i].volume / master_volume * volume;
            alSourcef(sources[i].source, AL_GAIN, sources[i].volume);
        }
    }
}

void VitaSoundSystem::SetMusicVolume(float volume) {
    music_volume = std::max(0.0f, std::min(1.0f, volume));
}

void VitaSoundSystem::SetSfxVolume(float volume) {
    sfx_volume = std::max(0.0f, std::min(1.0f, volume));
    
    // Update all active SFX sources
    for (int i = 0; i < MAX_SOUND_SOURCES; i++) {
        if (sources[i].in_use) {
            sources[i].volume = sources[i].volume / sfx_volume * volume;
            alSourcef(sources[i].source, AL_GAIN, sources[i].volume);
        }
    }
}

bool VitaSoundSystem::StartMusic(const char* filename, float volume, bool looping) {
    std::lock_guard<std::mutex> lock(queue_mutex);
    sound_queue.push({filename, volume, 1.0f, looping, -1});
    return true;
}

void VitaSoundSystem::StopMusic() {
    // Signal music to stop via queue
    std::lock_guard<std::mutex> lock(queue_mutex);
    sound_queue.push({"", 0, 1.0f, false, -2}); // Special stop command
}

void VitaSoundSystem::PauseMusic() {
    // Implementation for pausing
}

void VitaSoundSystem::ResumeMusic() {
    // Implementation for resuming
}

bool VitaSoundSystem::IsMusicPlaying() {
    // Check if music source is playing
    return false;
}

void VitaSoundSystem::Update() {
    ProcessSoundQueue();
    UpdateSources();
}

void VitaSoundSystem::MixerThread() {
    while (mixer_running) {
        UpdateSources();
        ProcessSoundQueue();
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}

void VitaSoundSystem::ProcessSoundQueue() {
    std::lock_guard<std::mutex> lock(queue_mutex);
    
    while (!sound_queue.empty()) {
        auto cmd = sound_queue.front();
        sound_queue.pop();
        
        if (cmd.source_id == -2) {
            // Stop music command
            StopAllSounds();
        } else if (!cmd.filename.empty()) {
            // Load and play music
            int buffer = LoadSound(cmd.filename.c_str());
            if (buffer >= 0) {
int source = PlaySound(buffer, cmd.volume * music_volume * master_volume, cmd.pitch, cmd.looping);
            if (source >= 0) {
                // Mark as music source
            }
            }
        }
    }
}

void VitaSoundSystem::UpdateSources() {
    for (int i = 0; i < MAX_SOUND_SOURCES; i++) {
        if (sources[i].in_use) {
            ALint state;
            alGetSourcei(sources[i].source, AL_SOURCE_STATE, &state);
            if (state == AL_STOPPED || state == AL_INITIAL) {
                alSourcei(sources[i].source, AL_BUFFER, 0);
                CheckOpenALError("alSourcei clear stopped");
                sources[i].in_use = false;
                sources[i].buffer = 0;
            }
        }
    }
}

// Legacy API
int Vita_LoadSound(const char* filename) {
    return g_vita_sound.LoadSound(filename);
}

int Vita_PlaySound(int buffer_id, float volume, float pitch, bool looping) {
    return g_vita_sound.PlaySound(buffer_id, volume, pitch, looping);
}

void Vita_StopSound(int source_id) {
    g_vita_sound.StopSound(source_id);
}

void Vita_StopAllSounds() {
    g_vita_sound.StopAllSounds();
}

void Vita_SetMasterVolume(float volume) {
    g_vita_sound.SetMasterVolume(volume);
}

void Vita_SetMusicVolume(float volume) {
    g_vita_sound.SetMusicVolume(volume);
}

void Vita_SetSfxVolume(float volume) {
    g_vita_sound.SetSfxVolume(volume);
}

int Vita_StartMusic(const char* filename, float volume, bool looping) {
    return g_vita_sound.StartMusic(filename, volume, looping) ? 0 : -1;
}

void Vita_StopMusic() {
    g_vita_sound.StopMusic();
}

void Vita_PauseMusic() {
    g_vita_sound.PauseMusic();
}

void Vita_ResumeMusic() {
    g_vita_sound.ResumeMusic();
}

void Vita_SoundUpdate() {
    g_vita_sound.Update();
}