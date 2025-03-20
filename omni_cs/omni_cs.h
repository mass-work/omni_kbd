#pragma once

#include "quantum.h"
#include "../drivers/cst816t.h"
#include "dynamic_keymap.h"
#include <stdint.h>

#define EEPROM_TOTAL_SIZE 4096
#define EEPROM_OMNI_RESERVATION (EEPROM_TOTAL_SIZE - sizeof(OmniData))
#define EEPROM_CUSTOM_DATA_ADDR (EEPROM_OMNI_RESERVATION - sizeof(CalibrationData) - sizeof(ColorData))
#define EEPROM_OMNI_TB (EEPROM_CUSTOM_DATA_ADDR)
#define EEPROM_OMNI_COLORS (EEPROM_OMNI_TB+ sizeof(CalibrationData))
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

typedef struct {
    uint8_t speed_adjust1;
    uint8_t slope_factor1;
    uint8_t speed_adjust2;
    uint8_t slope_factor2;
} CalibrationData;

typedef struct {
    uint8_t hue1;
    uint8_t sat1;
    uint8_t val1;
    uint8_t hue2;
    uint8_t sat2;
    uint8_t val2;       
    uint8_t hue3;
    uint8_t sat3;
    uint8_t val3;
} ColorData;

typedef struct {
    uint8_t reservation0;
    uint8_t reservation1;
    uint8_t reservation2;
    uint8_t reservation3;
    uint8_t reservation4;
    uint8_t reservation5;
    uint8_t reservation6;       
    uint8_t reservation7;
} OmniData;

enum custom_keycodes {
    KC_hue_bg_UP = SAFE_RANGE,
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
};

typedef enum {
    DISPLAY_MODE_TOUCH_KEY,
    DISPLAY_MODE_TRACKBALL_TUNING,
    DISPLAY_MODE_SWIPE_GESTURE,
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
