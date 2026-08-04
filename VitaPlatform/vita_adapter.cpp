#include "vita_platform.h"
#include "vita_alephone_config.h"
#include <SDL2/SDL.h>
#include <psp2/io/fcntl.h>
#include <psp2/io/stat.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/ctrl.h>
#include <psp2/touch.h>
#ifdef HAVE_OPENGL
#include <vitaGL.h>
#endif
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cmath>

#include "cseries.h"
#include "shell.h"
#include "preferences.h"
#include "joystick.h"
#include "screen.h"
#include "FileHandler.h"

extern DirectorySpecifier default_data_dir;
extern DirectorySpecifier local_data_dir;
extern DirectorySpecifier preferences_dir;
extern DirectorySpecifier saved_games_dir;
extern DirectorySpecifier quick_saves_dir;
extern DirectorySpecifier image_cache_dir;
extern DirectorySpecifier recordings_dir;
extern DirectorySpecifier screenshots_dir;
extern DirectorySpecifier log_dir;
extern std::vector<DirectorySpecifier> data_search_path;
extern input_preferences_data* input_preferences;
extern graphics_preferences_data* graphics_preferences;
extern bool displaying_fps;

static std::string g_vita_data_path;
static std::string g_vita_save_path;
static std::string g_vita_prefs_path;
static std::string g_vita_local_path;

static float vita_apply_stick_deadzone(float value)
{
    constexpr float deadzone = 0.14f;
    if (std::fabs(value) < deadzone)
        return 0.0f;

    const float sign = value < 0.0f ? -1.0f : 1.0f;
    float scaled = (std::fabs(value) - deadzone) / (1.0f - deadzone);
    if (scaled > 1.0f)
        scaled = 1.0f;
    return sign * scaled;
}

void vita_alephone_init_paths(void)
{
    g_vita_data_path = vita_get_data_path();
    g_vita_save_path = vita_get_save_path();
    g_vita_prefs_path = vita_get_preferences_path();
    g_vita_local_path = g_vita_data_path;

    default_data_dir = g_vita_data_path;
    local_data_dir = g_vita_local_path;
    preferences_dir = g_vita_prefs_path;
    saved_games_dir = g_vita_save_path;
    quick_saves_dir = g_vita_save_path + "quicksaves/";
    image_cache_dir = g_vita_data_path + "cache/";
    recordings_dir = g_vita_data_path + "recordings/";
    screenshots_dir = g_vita_data_path + "screenshots/";
    log_dir = g_vita_data_path + "logs/";

    data_search_path.clear();
    data_search_path.push_back(default_data_dir);
    data_search_path.push_back(local_data_dir);
    data_search_path.push_back(preferences_dir);
    data_search_path.push_back(saved_games_dir);

    sceIoMkdir(g_vita_save_path.c_str(), 0777);
    sceIoMkdir(g_vita_prefs_path.c_str(), 0777);
    sceIoMkdir(image_cache_dir.GetPath(), 0777);
    sceIoMkdir(recordings_dir.GetPath(), 0777);
    sceIoMkdir(screenshots_dir.GetPath(), 0777);
    sceIoMkdir(log_dir.GetPath(), 0777);
    sceIoMkdir(quick_saves_dir.GetPath(), 0777);
}

const char* vita_get_default_data_dir(void)
{
    return g_vita_data_path.c_str();
}

const char* vita_get_local_data_dir(void)
{
    return g_vita_local_path.c_str();
}

const char* vita_get_preferences_dir(void)
{
    return g_vita_prefs_path.c_str();
}

const char* vita_get_saved_games_dir(void)
{
    return g_vita_save_path.c_str();
}

void vita_alephone_setup_opengl(void)
{
#ifdef HAVE_OPENGL
    vglInitExtended(0, 960, 544, 0, SCE_GXM_MULTISAMPLE_NONE);
    glViewport(0, 0, 960, 544);
#endif
}

void vita_alephone_swap_buffers(void)
{
#ifdef HAVE_OPENGL
    vglSwapBuffers(GL_TRUE);
#endif
}

int vita_alephone_get_screen_width(void)
{
    return 960;
}

int vita_alephone_get_screen_height(void)
{
    return 544;
}

void vita_alephone_handle_vita_events(void)
{
    SceCtrlData ctrl;
    sceCtrlPeekBufferPositive(0, &ctrl, 1);

#ifdef A1_VITA_TEST_INVINCIBLE
    static bool prev_invincible_combo = false;
    const unsigned int invincible_combo =
        SCE_CTRL_LTRIGGER | SCE_CTRL_RTRIGGER | SCE_CTRL_SELECT;
    const bool invincible_combo_pressed =
        (ctrl.buttons & invincible_combo) == invincible_combo;
    if (invincible_combo_pressed && !prev_invincible_combo)
    {
        const bool enabled = !vita_test_invincible_enabled();
        vita_test_invincible_set(enabled);
        screen_printf(enabled ? "Test invincibility ON" : "Test invincibility OFF");
        vita_platform_log(enabled ? "Test invincibility ON" : "Test invincibility OFF");
    }
    prev_invincible_combo = invincible_combo_pressed;
#endif

    SceTouchData touch_front;
    sceTouchPeek(SCE_TOUCH_PORT_FRONT, &touch_front, 1);

    SceTouchData touch_back;
    sceTouchPeek(SCE_TOUCH_PORT_BACK, &touch_back, 1);

    static bool prev_touch_front[SCE_TOUCH_MAX_REPORT] = {false};
    static bool prev_touch_back[SCE_TOUCH_MAX_REPORT] = {false};

    for (unsigned int i = 0; i < touch_front.reportNum; i++) {
        const SceTouchReport* report = &touch_front.report[i];
        if (!prev_touch_front[i]) {
            vita_handle_touch_input(i, report->x, report->y, true);
            prev_touch_front[i] = true;
        }
    }
    for (unsigned int i = 0; i < SCE_TOUCH_MAX_REPORT; i++) {
        bool found = false;
        for (unsigned int j = 0; j < touch_front.reportNum; j++) {
            if (touch_front.report[j].id == i) {
                found = true;
                break;
            }
        }
        if (!found && prev_touch_front[i]) {
            vita_handle_touch_input(i, 0, 0, false);
            prev_touch_front[i] = false;
        }
    }

    for (unsigned int i = 0; i < touch_back.reportNum; i++) {
        const SceTouchReport* report = &touch_back.report[i];
        if (!prev_touch_back[i]) {
            vita_handle_rear_touch_input(i, report->x, report->y, true);
            prev_touch_back[i] = true;
        }
    }
    for (unsigned int i = 0; i < SCE_TOUCH_MAX_REPORT; i++) {
        bool found = false;
        for (unsigned int j = 0; j < touch_back.reportNum; j++) {
            if (touch_back.report[j].id == i) {
                found = true;
                break;
            }
        }
        if (!found && prev_touch_back[i]) {
            vita_handle_rear_touch_input(i, 0, 0, false);
            prev_touch_back[i] = false;
        }
    }

    (void)ctrl;
}

void vita_alephone_apply_performance_preferences(void)
{
    graphics_preferences->screen_mode.acceleration = _no_acceleration;
    graphics_preferences->screen_mode.high_resolution = true;
    graphics_preferences->screen_mode.draw_every_other_line = false;
    graphics_preferences->screen_mode.bit_depth = 32;
    graphics_preferences->screen_mode.width = 960;
    graphics_preferences->screen_mode.height = 544;
    graphics_preferences->screen_mode.auto_resolution = false;
    graphics_preferences->screen_mode.high_dpi = false;
    graphics_preferences->screen_mode.fullscreen = true;
    graphics_preferences->screen_mode.hud = true;
    graphics_preferences->screen_mode.fix_h_not_v = true;
    graphics_preferences->screen_mode.translucent_map = false;
    graphics_preferences->screen_mode.bobbing_type = BobbingType::weapon_only;
    graphics_preferences->software_alpha_blending = _sw_alpha_off;
    graphics_preferences->software_sdl_driver = _sw_driver_default;
    graphics_preferences->fps_target = 30;
    graphics_preferences->ephemera_quality = _ephemera_low;

    displaying_fps = true;
}

void vita_alephone_map_buttons(void)
{
    const auto button = [](SDL_GameControllerButton value) {
        return static_cast<SDL_Scancode>(AO_SCANCODE_BASE_JOYSTICK_BUTTON + value);
    };
    const auto axis_positive = [](SDL_GameControllerAxis value) {
        return static_cast<SDL_Scancode>(AO_SCANCODE_BASE_JOYSTICK_AXIS_POSITIVE + value);
    };
    const auto axis_negative = [](SDL_GameControllerAxis value) {
        return static_cast<SDL_Scancode>(AO_SCANCODE_BASE_JOYSTICK_AXIS_NEGATIVE + value);
    };

    /*
     * Native Vita layout.  SDL's generic defaults put fire on analog trigger
     * axes, which the Vita does not have, and use L/R for weapon switching.
     * Keep twin-stick movement/aiming and move both triggers to the physical
     * shoulders so the game is playable without editing preferences first.
     */
    for (int i = 0; i < 21; ++i)
        input_preferences->key_bindings[i].clear();

    input_preferences->key_bindings[0].insert(axis_negative(SDL_CONTROLLER_AXIS_LEFTY));  // move forward
    input_preferences->key_bindings[1].insert(axis_positive(SDL_CONTROLLER_AXIS_LEFTY));  // move backward
    input_preferences->key_bindings[2].insert(axis_negative(SDL_CONTROLLER_AXIS_RIGHTX)); // turn left
    input_preferences->key_bindings[3].insert(axis_positive(SDL_CONTROLLER_AXIS_RIGHTX)); // turn right
    input_preferences->key_bindings[4].insert(axis_negative(SDL_CONTROLLER_AXIS_LEFTX));  // sidestep left
    input_preferences->key_bindings[5].insert(axis_positive(SDL_CONTROLLER_AXIS_LEFTX));  // sidestep right
    input_preferences->key_bindings[6].insert(button(SDL_CONTROLLER_BUTTON_DPAD_LEFT));    // glance left
    input_preferences->key_bindings[7].insert(button(SDL_CONTROLLER_BUTTON_DPAD_RIGHT));   // glance right
    input_preferences->key_bindings[8].insert(axis_negative(SDL_CONTROLLER_AXIS_RIGHTY)); // look up
    input_preferences->key_bindings[9].insert(axis_positive(SDL_CONTROLLER_AXIS_RIGHTY)); // look down
    input_preferences->key_bindings[10].insert(button(SDL_CONTROLLER_BUTTON_Y));           // recenter view / Triangle
    input_preferences->key_bindings[11].insert(button(SDL_CONTROLLER_BUTTON_DPAD_UP));     // previous weapon
    input_preferences->key_bindings[12].insert(button(SDL_CONTROLLER_BUTTON_DPAD_DOWN));   // next weapon
    input_preferences->key_bindings[13].insert(button(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER)); // primary fire / R
    input_preferences->key_bindings[14].insert(button(SDL_CONTROLLER_BUTTON_LEFTSHOULDER));  // secondary fire / L
    input_preferences->key_bindings[16].insert(button(SDL_CONTROLLER_BUTTON_B));            // run/swim / Circle
    input_preferences->key_bindings[16].insert(button(SDL_CONTROLLER_BUTTON_LEFTSTICK));
    input_preferences->key_bindings[18].insert(button(SDL_CONTROLLER_BUTTON_A));            // action / Cross
    input_preferences->key_bindings[19].insert(button(SDL_CONTROLLER_BUTTON_X));            // automap / Square
    input_preferences->key_bindings[20].insert(button(SDL_CONTROLLER_BUTTON_RIGHTSTICK));   // auxiliary / R3

    input_preferences->controller_analog = true;
    input_preferences->controller_aim_inverted = false;
    input_preferences->controller_sensitivity_horizontal = FIXED_ONE;
    input_preferences->controller_sensitivity_vertical = FIXED_ONE;
    input_preferences->controller_deadzone_horizontal = 4096;
    input_preferences->controller_deadzone_vertical = 4096;

    input_preferences->shell_key_bindings[_key_inventory_left].clear();
    input_preferences->shell_key_bindings[_key_inventory_right].clear();
    input_preferences->shell_key_bindings[_key_switch_view].clear();
    input_preferences->shell_key_bindings[_key_volume_up].clear();
    input_preferences->shell_key_bindings[_key_volume_down].clear();
    input_preferences->shell_key_bindings[_key_zoom_in].clear();
    input_preferences->shell_key_bindings[_key_zoom_out].clear();
    input_preferences->shell_key_bindings[_key_toggle_fps].clear();
    input_preferences->shell_key_bindings[_key_activate_console].clear();
    input_preferences->shell_key_bindings[_key_show_scores].clear();

    input_preferences->shell_key_bindings[_key_inventory_left].insert(button(SDL_CONTROLLER_BUTTON_DPAD_LEFT));
    input_preferences->shell_key_bindings[_key_inventory_right].insert(button(SDL_CONTROLLER_BUTTON_DPAD_RIGHT));
    input_preferences->shell_key_bindings[_key_zoom_in].insert(button(SDL_CONTROLLER_BUTTON_DPAD_UP));
    input_preferences->shell_key_bindings[_key_zoom_out].insert(button(SDL_CONTROLLER_BUTTON_DPAD_DOWN));
    input_preferences->shell_key_bindings[_key_show_scores].insert(button(SDL_CONTROLLER_BUTTON_BACK));
}
