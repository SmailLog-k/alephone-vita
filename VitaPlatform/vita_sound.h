/*
 * Vita Sound System - Minimal OpenAL implementation for Aleph One
 * Bypasses problematic Boost lockfree/atomic code
 */

#ifndef VITA_SOUND_H
#define VITA_SOUND_H

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>
#include <string>
#include <vector>
#include <mutex>
#include <thread>
#include <queue>

#define MAX_SOUND_SOURCES 32
#define MAX_SOUND_BUFFERS 256
#define MAX_QUEUED_BUFFERS 4

struct VitaSoundBuffer {
    ALuint buffer = 0;
    std::string filename;
    bool loaded = false;
    int channels = 1;
    int sample_rate = 22050;
    int bits = 16;
};

struct VitaSoundSource {
    ALuint source = 0;
    ALuint buffer = 0;
    bool in_use = false;
    float volume = 1.0f;
    float pitch = 1.0f;
    bool looping = false;
    int buffer_queue[MAX_QUEUED_BUFFERS];
    int queue_head = 0;
    int queue_tail = 0;
    int queue_count = 0;
    int source_id = 0;
};

class VitaSoundSystem {
private:
    ALCdevice* device = nullptr;
    ALCcontext* context = nullptr;
    
    std::vector<VitaSoundBuffer> buffers;
    std::vector<VitaSoundSource> sources;
    std::mutex sound_mutex;
    
    // Audio mixing thread
    std::thread mixer_thread;
    bool mixer_running = false;
    
    // Sound data queue for streaming
    struct QueuedSound {
        std::string filename;
        float volume;
        float pitch;
        bool looping;
        int source_id;
    };
    std::queue<QueuedSound> sound_queue;
    std::mutex queue_mutex;
    
    // Volume control
    float master_volume = 1.0f;
    float music_volume = 1.0f;
    float sfx_volume = 1.0f;
    
public:
    VitaSoundSystem();
    ~VitaSoundSystem();
    
    bool Initialize();
    void Shutdown();
    
    // Sound buffer management
    int LoadSound(const char* filename);
    void UnloadSound(int buffer_id);
    
    // Sound playback
    int PlaySound(int buffer_id, float volume = 1.0f, float pitch = 1.0f, bool looping = false);
    int PlaySound3D(int buffer_id, float x, float y, float z, float volume = 1.0f, float pitch = 1.0f, bool looping = false);
    void StopSound(int source_id);
    void StopAllSounds();
    
    // Source control
    void SetSourceVolume(int source_id, float volume);
    void SetSourcePitch(int source_id, float pitch);
    void SetSourcePosition(int source_id, float x, float y, float z);
    void SetSourceVelocity(int source_id, float x, float y, float z);
    bool IsSourcePlaying(int source_id);
    
    // Listener control
    void SetListenerPosition(float x, float y, float z);
    void SetListenerVelocity(float x, float y, float z);
    void SetListenerOrientation(float at_x, float at_y, float at_z, float up_x, float up_y, float up_z);
    
    // Volume control
    void SetMasterVolume(float volume);
    void SetMusicVolume(float volume);
    void SetSfxVolume(float volume);
    float GetMasterVolume() const { return master_volume; }
    float GetMusicVolume() const { return music_volume; }
    float GetSfxVolume() const { return sfx_volume; }
    
    // Streaming music
    bool StartMusic(const char* filename, float volume = 1.0f, bool looping = true);
    void StopMusic();
    void PauseMusic();
    void ResumeMusic();
    bool IsMusicPlaying();
    
    // Update - call from main loop
    void Update();
    
private:
    void MixerThread();
    void ProcessSoundQueue();
    void UpdateSources();
    void CheckOpenALError(const char* operation);
    int FindFreeSource();
    int FindFreeBuffer();
    bool LoadWAVFile(const char* filename, VitaSoundBuffer& buffer);
    bool LoadOGGFile(const char* filename, VitaSoundBuffer& buffer);
};

// Global instance
extern VitaSoundSystem g_vita_sound;

// Legacy API compatibility
int Vita_LoadSound(const char* filename);
int Vita_PlaySound(int buffer_id, float volume = 1.0f, float pitch = 1.0f, bool looping = false);
void Vita_StopSound(int source_id);
void Vita_StopAllSounds();
void Vita_SetMasterVolume(float volume);
void Vita_SetMusicVolume(float volume);
void Vita_SetSfxVolume(float volume);
int Vita_StartMusic(const char* filename, float volume = 1.0f, bool looping = true);
void Vita_StopMusic();
void Vita_PauseMusic();
void Vita_ResumeMusic();
void Vita_SoundUpdate();

#endif