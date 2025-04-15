#pragma once

#include "quantum.h"
#include "../drivers/cst816t.h"
#include <stdint.h>

#define DEFAULT_SPEED_ADJUST1 1.6f
#define DEFAULT_SLOPE_FACTOR1 50
#define DEFAULT_SPEED_ADJUST2 1.4f
#define DEFAULT_SLOPE_FACTOR2 70
#define ENABLE_TOUCH_UPDATE 1 // タッチキー押下時の画面更新 1:有効、0:無効
#define LONG_TOUCH_TIME 500
#define SWIPE_GESTURE_DEBOUNCE_TIME 100
#define TOUCH_DEBOUNCE_TIME 200
#define SINGLE_CLICK_TIME 100
#define SLEEPING_KB_TIME 60000
#define SLEEP_VIEW 1 // 1: code rain

typedef enum {
    TRACKBALL_CURSOR = 0,
    TRACKBALL_TAP    = 1,
} trackball_mode_t;

const char *get_layer_name(uint8_t layer); 

enum custom_keycodes {
    KC_hue_bg_UP = QK_KB_0,
    KC_hue_bg_DOWN,
    KC_sat_bg_UP,
    KC_sat_bg_DOWN,
    KC_val_bg_UP,
    KC_val_bg_DOWN,
    KC_hue_main_color_UP,
    KC_hue_main_color_DOWN,
    KC_sat_main_color_UP,
    KC_sat_main_color_DOWN,
    KC_val_main_color_UP,
    KC_val_main_color_DOWN,
    KC_hue_sub_color_UP,
    KC_hue_sub_color_DOWN,
    KC_sat_sub_color_UP,
    KC_sat_sub_color_DOWN,
    KC_val_sub_color_UP,
    KC_val_sub_color_DOWN,
    AUTO_MOUSE_TOGGLE,
    TB_R_MODE_TOGGLE,
    TB_L_MODE_TOGGLE,
    KC_DP_TOUCH_KEY,
    KC_DP_TB_TUNE,
    KC_DP_SWIPE_GESTURE,
    KC_DP_KEY_MAT,
};

typedef enum {
    DISPLAY_MODE_TOUCH_KEY,
    DISPLAY_MODE_TRACKBALL_TUNING,
    DISPLAY_MODE_SWIPE_GESTURE,
    DISPLAY_MODE_KEY_MATRIX,
} display_mode_t;

typedef struct {
    int16_t x;
    int16_t y;
} point_t;
extern point_t circles[6];

typedef struct {
    painter_image_handle_t *image;
    int x;
    int y;
} ImagePosition;

extern uint8_t hue_bg;
extern uint8_t sat_bg;
extern uint8_t val_bg;
extern uint8_t hue_main_color;
extern uint8_t sat_main_color;
extern uint8_t val_main_color;
extern uint8_t hue_sub_color;
extern uint8_t sat_sub_color;
extern uint8_t val_sub_color;
extern painter_font_handle_t noto11_font;
extern uint16_t touch_x;
extern uint16_t touch_y;
extern bool touch_signal;
extern uint8_t current_lcd_layer;
extern uint8_t current_lcd_category;
extern uint16_t virtual_keycode[72];
extern display_mode_t display_mode;

void switch_display_mode(void);
void display_redraw(void);
