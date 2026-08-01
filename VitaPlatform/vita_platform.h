#ifndef VITA_PLATFORM_H
#define VITA_PLATFORM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

void vita_platform_init(void);
void vita_platform_shutdown(void);
void vita_platform_log(const char* message);

const char* vita_get_data_path(void);
const char* vita_get_save_path(void);
const char* vita_get_preferences_path(void);

void vita_set_screen_mode(int width, int height, bool fullscreen);
void vita_get_screen_mode(int* width, int* height, bool* fullscreen);

void vita_handle_touch_input(int touch_id, float x, float y, bool pressed);
void vita_handle_rear_touch_input(int touch_id, float x, float y, bool pressed);
void vita_handle_analog_input(int stick_id, float x, float y);

void vita_alephone_init_paths(void);
void vita_alephone_setup_opengl(void);
void vita_alephone_swap_buffers(void);
int vita_alephone_get_screen_width(void);
int vita_alephone_get_screen_height(void);
void vita_alephone_handle_vita_events(void);
void vita_alephone_apply_performance_preferences(void);
void vita_alephone_map_buttons(void);

#ifdef __cplusplus
}
#endif

#endif
