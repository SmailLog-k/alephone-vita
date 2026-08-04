#include "vita_platform.h"
#include "vita_alephone_config.h"
#include <psp2/io/fcntl.h>
#include <psp2/io/stat.h>
#include <psp2/ctrl.h>
#include <psp2/touch.h>
#include <psp2/display.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/kernel/sysmem.h>
#include <psp2/power.h>
#ifdef HAVE_OPENGL
#include <vitaGL.h>
#endif
#include <SDL2/SDL.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static char g_data_path[256];
static char g_save_path[256];
static char g_prefs_path[256];
static int g_screen_width = 960;
static int g_screen_height = 544;
static bool g_fullscreen = true;
static FILE* g_log_file = NULL;
static bool g_test_invincible = false;

void vita_platform_log(const char* message)
{
    if (!message) return;

    if (g_log_file) {
        fprintf(g_log_file, "%s\n", message);
        fflush(g_log_file);
    }
}

bool vita_test_invincible_enabled(void)
{
#ifdef A1_VITA_TEST_INVINCIBLE
    return g_test_invincible;
#else
    return false;
#endif
}

void vita_test_invincible_set(bool enabled)
{
#ifdef A1_VITA_TEST_INVINCIBLE
    g_test_invincible = enabled;
#else
    (void)enabled;
#endif
}

void vita_platform_init(void)
{
    snprintf(g_data_path, sizeof(g_data_path), "%s/", A1_VITA_DATA_DIR);
    snprintf(g_save_path, sizeof(g_save_path), "%ssaves/", g_data_path);
    snprintf(g_prefs_path, sizeof(g_prefs_path), "%sprefs/", g_data_path);

    sceIoMkdir(g_data_path, 0777);
    sceIoMkdir(g_save_path, 0777);
    sceIoMkdir(g_prefs_path, 0777);

    char log_path[320];
    snprintf(log_path, sizeof(log_path), "%salephone.log", g_data_path);
    g_log_file = fopen(log_path, "w");
    vita_platform_log("Aleph One Vita: entered vita_platform_init");

    scePowerSetArmClockFrequency(444);
    scePowerSetBusClockFrequency(222);
    scePowerSetGpuClockFrequency(222);
    scePowerSetGpuXbarClockFrequency(166);
    vita_platform_log("Aleph One Vita: requested maximum clocks");

    sceTouchSetSamplingState(SCE_TOUCH_PORT_FRONT, SCE_TOUCH_SAMPLING_STATE_START);
    sceTouchSetSamplingState(SCE_TOUCH_PORT_BACK, SCE_TOUCH_SAMPLING_STATE_START);

    sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG);
    sceCtrlSetSamplingModeExt(SCE_CTRL_MODE_ANALOG_WIDE);

#ifdef HAVE_OPENGL
    vglInitExtended(0, 960, 544, 0, SCE_GXM_MULTISAMPLE_NONE);
#endif

    vita_platform_log("Aleph One Vita: platform input initialized");
}

void vita_platform_shutdown(void)
{
    vita_platform_log("Aleph One Vita: shutting down");
    sceTouchSetSamplingState(SCE_TOUCH_PORT_FRONT, SCE_TOUCH_SAMPLING_STATE_STOP);
    sceTouchSetSamplingState(SCE_TOUCH_PORT_BACK, SCE_TOUCH_SAMPLING_STATE_STOP);
    // vglEnd() not available in vitaGL

    if (g_log_file) {
        fclose(g_log_file);
        g_log_file = NULL;
    }
}

const char* vita_get_data_path(void)
{
    return g_data_path;
}

const char* vita_get_save_path(void)
{
    return g_save_path;
}

const char* vita_get_preferences_path(void)
{
    return g_prefs_path;
}

void vita_set_screen_mode(int width, int height, bool fullscreen)
{
    g_screen_width = width;
    g_screen_height = height;
    g_fullscreen = fullscreen;
    // vglResize not available in vitaGL
}

void vita_get_screen_mode(int* width, int* height, bool* fullscreen)
{
    *width = g_screen_width;
    *height = g_screen_height;
    *fullscreen = g_fullscreen;
}

void vita_handle_touch_input(int touch_id, float x, float y, bool pressed)
{
    SDL_Event event;
    memset(&event, 0, sizeof(event));
    event.type = pressed ? SDL_FINGERDOWN : SDL_FINGERUP;
    event.tfinger.touchId = touch_id;
    event.tfinger.fingerId = touch_id;
    event.tfinger.x = x / 960.0f;
    event.tfinger.y = y / 544.0f;
    event.tfinger.dx = 0;
    event.tfinger.dy = 0;
    event.tfinger.pressure = pressed ? 1.0f : 0.0f;
    SDL_PushEvent(&event);
}

void vita_handle_rear_touch_input(int touch_id, float x, float y, bool pressed)
{
    SDL_Event event;
    memset(&event, 0, sizeof(event));
    event.type = pressed ? SDL_FINGERDOWN : SDL_FINGERUP;
    event.tfinger.touchId = touch_id + 10;
    event.tfinger.fingerId = touch_id + 10;
    event.tfinger.x = x / 960.0f;
    event.tfinger.y = y / 544.0f;
    event.tfinger.dx = 0;
    event.tfinger.dy = 0;
    event.tfinger.pressure = pressed ? 1.0f : 0.0f;
    SDL_PushEvent(&event);
}

void vita_handle_analog_input(int stick_id, float x, float y)
{
    SDL_Event event;
    memset(&event, 0, sizeof(event));
    event.type = SDL_CONTROLLERAXISMOTION;
    event.caxis.which = 0;
    if (stick_id == 0) {
        event.caxis.axis = SDL_CONTROLLER_AXIS_LEFTX;
        event.caxis.value = (Sint16)(x * 32767.0f);
    } else if (stick_id == 1) {
        event.caxis.axis = SDL_CONTROLLER_AXIS_LEFTY;
        event.caxis.value = (Sint16)(y * 32767.0f);
    } else if (stick_id == 2) {
        event.caxis.axis = SDL_CONTROLLER_AXIS_RIGHTX;
        event.caxis.value = (Sint16)(x * 32767.0f);
    } else if (stick_id == 3) {
        event.caxis.axis = SDL_CONTROLLER_AXIS_RIGHTY;
        event.caxis.value = (Sint16)(y * 32767.0f);
    }
    SDL_PushEvent(&event);
}
