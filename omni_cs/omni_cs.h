#pragma once

#include "quantum.h"
#include "../drivers/cst816t.h" // `cst816t_XY`型が定義されているヘッダーファイル

// EEPROMの保存データのアドレス
#define EEPROM_OMNI_TB (EECONFIG_USER + 0)

#define DEFAULT_SPEED_ADJUST1 1.6f
#define DEFAULT_SLOPE_FACTOR1 50
#define DEFAULT_SPEED_ADJUST2 1.4f
#define DEFAULT_SLOPE_FACTOR2 70

#define ENABLE_TOUCH_UPFDATE 1 // タッチキー押下時の画面更新 1:有効、0:無効
#define LONG_TOUCH_TIME 500
#define TOUCH_DEBOUNCE_TIME 200
#define SINGLE_CLICK_TIME 100
#define SLEEPING_KB_TIME 300000 //300000

typedef enum {
    DISPLAY_MODE_TOUCH_KEY,
    DISPLAY_MODE_TRACKBALL_TUNING,
} display_mode_t;

// 保存するデータ構造
typedef struct {
    uint8_t speed_adjust1;  // 10倍した値を保存
    uint8_t slope_factor1;
    uint8_t speed_adjust2;  // 10倍した値を保存
    uint8_t slope_factor2;
} CalibrationData;

extern uint16_t touch_x;
extern uint16_t touch_y;
extern bool touch_signal;
extern uint8_t current_lcd_layer;
extern uint8_t current_lcd_category;
extern uint16_t virtual_keycode[72];

extern display_mode_t display_mode;

void switch_display_mode(void);

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
