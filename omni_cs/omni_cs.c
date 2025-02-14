// Copyright 2025 mass
// SPDX-License-Identifier: GPL-2.0-or-later

#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <print.h>
#include "wait.h"
#include "timer.h"
#include "i2c_master.h"
#include "gpio.h"
#include "qp.h"
#include "eeprom.h"
#include "usb_main.h"
#include "usb_driver.h"
// #include "usb_device_state.h"
#include "config.h"
#include "omni_cs.h"
#include "drivers/pmw33xx_common.h"
#include "drivers/cst816t.h"
#include "font/noto11.qff.h"
#include "customfunc/draw_custom.h"
#include "icon/omni_image_loader.h"
#include "matrix.h"

uint32_t sleeping_timer;
bool matrix_changed = false;
bool sleeping_state = false;

#define MACRO_KEY_START 0x7700
#define MACRO_KEY_END   0x77FE
#define MACRO_KEY_COUNT (MACRO_KEY_END - MACRO_KEY_START + 1)
#define constrain_hid(amt) ((amt) < -127 ? -127 : ((amt) > 127 ? 127 : (amt)))

// pmw3360
static float accumulated_x = 0.0f;
static float accumulated_y = 0.0f;
static float accumulated_h = 0.0f;
static float accumulated_v = 0.0f;
// touch display
static painter_device_t display;
painter_font_handle_t noto11_font;
painter_image_handle_t qp_load_image_mem(const void *buffer);
painter_image_handle_t image_logo;
painter_image_handle_t image_save;
painter_image_handle_t layer_00, layer_01, layer_02, layer_03, layer_04, layer_05, layer_06, layer_07, layer_08, layer_09, layer_10, layer_11;
painter_image_handle_t image_000, image_001, image_002, image_003, image_004, image_005, image_006, image_007, image_008, image_009, image_010, image_011, image_012, image_013, image_014, image_015, image_016, image_017, image_018, image_019, image_020, image_021, image_022, image_023, image_024, image_025, image_026, image_027, image_028, image_029, image_030, image_031, image_032, image_033, image_034, image_035, image_036, image_037, image_038, image_039, image_040, image_041, image_042, image_043, image_044, image_045, image_046, image_047, image_048, image_049, image_050, image_051, image_052, image_053, image_054, image_055, image_056, image_057, image_058, image_059, image_060, image_061, image_062, image_063, image_064, image_065, image_066, image_067, image_068, image_069, image_070, image_071, image_072, image_073, image_074, image_075, image_076, image_077, image_078, image_079, image_080, image_081, image_082, image_083, image_084, image_085, image_086, image_087, image_088, image_089, image_090, image_091, image_092, image_093, image_094, image_095, image_096, image_097, image_098, image_099, image_100, image_101, image_102, image_103, image_104, image_105, image_106, image_107, image_108, image_109, image_110, image_111, image_112, image_113, image_114, image_115, image_116, image_117, image_118, image_119, image_120, image_121, image_122, image_123, image_124, image_125, image_126, image_127, image_128, image_129, image_130, image_131, image_132, image_133, image_134, image_135, image_136, image_137, image_138, image_139, image_140, image_141, image_142, image_143, image_144, image_145, image_146, image_147, image_148, image_149, image_150, image_151, image_152, image_153, image_154, image_155, image_156, image_157, image_158, image_159, image_160, image_161, image_162, image_163, image_164, image_165, image_166, image_167, image_168, image_169, image_170, image_171, image_172, image_173, image_174, image_175, image_176, image_177, image_178, image_179, image_180, image_181, image_182, image_183, image_184, image_185, image_186, image_187, image_188, image_189, image_190, image_191, image_192, image_193, image_194, image_195, image_196, image_197, image_198, image_199, image_200, image_201, image_202, image_203, image_204, image_205, image_206, image_207, image_208, image_209, image_210, image_211, image_212, image_213, image_214, image_215, image_216, image_217, image_218, image_219, image_220, image_221, image_222, image_223, image_224, image_225, image_226, image_227, image_228, image_229, image_230, image_231, image_232, image_233, image_234, image_235, image_236, image_237, image_238, image_239, image_240, image_241, image_242, image_243, image_244, image_245, image_246, image_247, image_248, image_249, image_250, image_251, image_252, image_253, image_254;
bool is_first_frame = true;  // 初回描画のフラグ
bool is_second_frame = true;  // 初回描画のフラグ
static deferred_token my_anim;
static uint32_t last_update = 0;
const uint32_t update_interval = 50;  // 50ms間隔で描画
uint32_t lcd_current_time = 0;
uint32_t lcd_fast_res_time = 0;
uint8_t touch_data[6];
ImagePosition images[7];
ImagePosition lcd_layer_app_images[MAX_LCD_CATEGORY + 1][MAX_LCD_LAYER + 1][7];
uint8_t current_lcd_layer = 0;       // 初期レイヤー
uint8_t current_lcd_category = 0;    // 初期カテゴリー
point_t home_point = {120, 120};
point_t circles[] = {{200, 120}, {160, 189}, {80, 189}, {40, 120}, {80, 51}, {160, 51}};
uint8_t gesture_id = GESTURE_NONE;
// 描画とタッチ処理
uint16_t touch_x = 0xFFFF;
uint16_t touch_y = 0xFFFF;
void process_touch_trackball_tuning_mode(uint16_t touch_x, uint16_t touch_y);
static uint16_t long_touch_time = 0;//何秒長押ししたかを判定
static uint16_t last_touch_time = 0;//何秒長押ししたかを判定
// other
uint16_t current_time = 0; // 現在の時間を取得
static uint16_t last_tap_time = 0;
bool touch_signal = false;
bool touch_signal_view_updata = false;
uint16_t virtual_keycode[KEYCODE_SIZE];
uint16_t pre_virtual_keycode[KEYCODE_SIZE];
uint16_t import_keymaps[MATRIX_ROWS / 2][MATRIX_COLS];
uint16_t cpi = 800;    // トラックボールの解像度

// Graph位置
float speed_adjust1; // 補正パラメータ
float pre_speed_adjust1;
int slope_factor1; // 補正パラメータ
int pre_slope_factor1; // 補正パラメータ
uint8_t hue1 = 220;     // 色相
uint8_t sat1 = 255;     // 彩度
uint8_t val1 = 255;     // 明度
uint8_t x_pos1 = 170;     // 明度
uint8_t y_pos1 = 110;     // 明度

float speed_adjust2; // 補正パラメータ
float pre_speed_adjust2;
int slope_factor2; // 補正パラメータ
int pre_slope_factor2; // 補正パラメータ
uint8_t hue2 = 135;     // 色相
uint8_t sat2 = 255;     // 彩度
uint8_t val2 = 255;     // 明度
uint8_t x_pos2 = 70;     // 明度
uint8_t y_pos2 = 110;     // 明度

//+-位置
uint8_t y_pos3 = 70;
uint8_t y_pos4 = 165;
static int16_t text1_width, text3_width, text4_width, text5_width, text6_width = 0;

// ディスプレイの初期状態を定義
display_mode_t display_mode =  DISPLAY_MODE_TOUCH_KEY;
CalibrationData calibration_data;
bool enable_touch_update = true;

static uint32_t blink_start_time = 0;
static bool is_backlight_off = false;
bool lcd_is_on = true;
uint32_t last_usb_check_time = 0;

bool single_click_flag = false;
uint8_t single_click_counter = 0;
uint16_t single_click_timer;
uint16_t single_touch_x;
uint16_t single_touch_y;

static const char *text1 = "TB TUNE";
static const char *text3 = "Right";
static const char *text4 = "Left";
static const char *text5 = "+      +";
static const char *text6 = "-      -";

void process_cursor_report(report_mouse_t *mouse_report, pmw33xx_report_t report, float speed_adjust, uint8_t slope_factor){
    if (!report.motion.b.is_lifted) {
        int cpi_scale = 2; //後でCPIの設定含め再検討
        float x = (report.delta_x / cpi_scale);
        float y = (report.delta_y / cpi_scale);
        int sign_x = (x > 0) - (x < 0);
        int sign_y = (y > 0) - (y < 0);
        float x_corr = pow(fabs(x), speed_adjust) / pow(127, speed_adjust) * 127 / 100 * slope_factor * sign_x;
        float y_corr = pow(fabs(y), speed_adjust) / pow(127, speed_adjust) * 127 / 100 * slope_factor * sign_y;
        accumulated_x += x_corr;
        accumulated_y += y_corr;
        if (fabs(accumulated_x) >= 1.0f) {
            mouse_report->x = -constrain_hid(mouse_report->x + accumulated_x);
            accumulated_x = 0;
        }
        if (fabs(accumulated_y) >= 1.0f) {
            mouse_report->y = constrain_hid(mouse_report->y + accumulated_y);
            accumulated_y = 0;
        }
    }
}

void process_tap_report(report_mouse_t *mouse_report, pmw33xx_report_t report, float speed_adjust, uint8_t slope_factor) {
    if (!report.motion.b.is_lifted) {
        int cpi_scale = 40; // 後でCPIの設定含め再検討
        int x = (report.delta_x / cpi_scale);
        int y = (report.delta_y / cpi_scale);
        int sign_x = (x > 0) - (x < 0);
        int sign_y = (y > 0) - (y < 0);
        float x_corr = pow(fabs(x), speed_adjust) / pow(127, speed_adjust) * 127 / 100 * slope_factor * sign_x;
        float y_corr = pow(fabs(y), speed_adjust) / pow(127, speed_adjust) * 127 / 100 * slope_factor * sign_y;
        const float diagonal_limit = 0.6f; // tan(26.57°) ≈ 0.5, 斜め動作を許容する範囲を狭める
        float ratio = fabs(y_corr) / fabs(x_corr);
        if (ratio > diagonal_limit && ratio < (1.0f / diagonal_limit)) {
            return;
        } else if (ratio <= diagonal_limit) {
            accumulated_h += x_corr / 2;
        } else {
            accumulated_v += y_corr / 1;
        }
        int tap_cycle_max = 20;
        if (fabs(accumulated_h) >= 1.0f) {
            int tap_cycle_h = round(fabs(accumulated_h));
            tap_cycle_h = (tap_cycle_h > tap_cycle_max) ? tap_cycle_max : tap_cycle_h;
            for (int i = 0; i < tap_cycle_h; i += 2) {
                if (accumulated_h > 0) {
                    tap_code(KC_MS_WH_RIGHT);
                    // tap_code(KC_RIGHT);
                } else if (accumulated_h < 0){
                    tap_code(KC_MS_WH_LEFT);
                    // tap_code(KC_LEFT);
                }
            }
            accumulated_h = 0.0f; // ループ終了後にリセット
        }
        if (fabs(accumulated_v) >= 1.0f) {
            int tap_cycle_v = round(fabs(accumulated_v));
            tap_cycle_v = (tap_cycle_v > tap_cycle_max) ? tap_cycle_max : tap_cycle_v;
            for (int i = 0; i < tap_cycle_v; i += 2) {
                if (accumulated_v > 0) {
                    tap_code(KC_MS_WH_UP);
                    // tap_code(KC_UP);
                } else if (accumulated_v < 0){
                    tap_code(KC_MS_WH_DOWN);
                    // tap_code(KC_DOWN);
                }
            }
            accumulated_v = 0;
        }
    }
}

painter_image_handle_t* get_layer_img_func(uint16_t layer_count) {
    static painter_image_handle_t* layer_image_ptrs[12] = {
        &layer_00, &layer_01, &layer_02, &layer_03, &layer_04, &layer_05, &layer_06, &layer_07, &layer_08, &layer_09, &layer_10, &layer_11
    };
    return layer_image_ptrs[layer_count];
}

painter_image_handle_t* get_img_func(uint16_t keycode) {
    static painter_image_handle_t* image_ptrs[MACRO_KEY_COUNT] = {
        &image_000, &image_001, &image_002, &image_003, &image_004, &image_005, &image_006, &image_007, &image_008, &image_009, &image_010, &image_011, &image_012, &image_013, &image_014, &image_015, &image_016, &image_017, &image_018, &image_019, &image_020, &image_021, &image_022, &image_023, &image_024, &image_025, &image_026, &image_027, &image_028, &image_029, &image_030, &image_031, &image_032, &image_033, &image_034, &image_035, &image_036, &image_037, &image_038, &image_039, &image_040, &image_041, &image_042, &image_043, &image_044, &image_045, &image_046, &image_047, &image_048, &image_049, &image_050, &image_051, &image_052, &image_053, &image_054, &image_055, &image_056, &image_057, &image_058, &image_059, &image_060, &image_061, &image_062, &image_063, &image_064, &image_065, &image_066, &image_067, &image_068, &image_069, &image_070, &image_071, &image_072, &image_073, &image_074, &image_075, &image_076, &image_077, &image_078, &image_079, &image_080, &image_081, &image_082, &image_083, &image_084, &image_085, &image_086, &image_087, &image_088, &image_089, &image_090, &image_091, &image_092, &image_093, &image_094, &image_095, &image_096, &image_097, &image_098, &image_099, &image_100, &image_101, &image_102, &image_103, &image_104, &image_105, &image_106, &image_107, &image_108, &image_109, &image_110, &image_111, &image_112, &image_113, &image_114, &image_115, &image_116, &image_117, &image_118, &image_119, &image_120, &image_121, &image_122, &image_123, &image_124, &image_125, &image_126, &image_127, &image_128, &image_129, &image_130, &image_131, &image_132, &image_133, &image_134, &image_135, &image_136, &image_137, &image_138, &image_139, &image_140, &image_141, &image_142, &image_143, &image_144, &image_145, &image_146, &image_147, &image_148, &image_149, &image_150, &image_151, &image_152, &image_153, &image_154, &image_155, &image_156, &image_157, &image_158, &image_159, &image_160, &image_161, &image_162, &image_163, &image_164, &image_165, &image_166, &image_167, &image_168, &image_169, &image_170, &image_171, &image_172, &image_173, &image_174, &image_175, &image_176, &image_177, &image_178, &image_179, &image_180, &image_181, &image_182, &image_183, &image_184, &image_185, &image_186, &image_187, &image_188, &image_189, &image_190, &image_191, &image_192, &image_193, &image_194, &image_195, &image_196, &image_197, &image_198, &image_199, &image_200, &image_201, &image_202, &image_203, &image_204, &image_205, &image_206, &image_207, &image_208, &image_209, &image_210, &image_211, &image_212, &image_213, &image_214, &image_215, &image_216, &image_217, &image_218, &image_219, &image_220, &image_221, &image_222, &image_223, &image_224, &image_225, &image_226, &image_227, &image_228, &image_229, &image_230, &image_231, &image_232, &image_233, &image_234, &image_235, &image_236, &image_237, &image_238, &image_239, &image_240, &image_241, &image_242, &image_243, &image_244, &image_245, &image_246, &image_247, &image_248, &image_249, &image_250, &image_251, &image_252, &image_253, &image_254
    };

    if (keycode >= MACRO_KEY_START && keycode <= MACRO_KEY_END) {
        return image_ptrs[keycode - MACRO_KEY_START];
    }

    return &image_001; // デフォルト画像
}

void draw_background(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
    qp_rect(display, x1, y1, x2, y2, 0, 0, 0, true);  // HSV: H=0, S=0, V=0 (黒色)
}

void draw_background_all(void) {
    qp_rect(display, 0, 0, 240, 240, 0, 0, 0, true);  // HSV: H=0, S=0, V=0 (黒色)
}

void initialize_lcd_layer_app_images(void) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            lcd_layer_app_images[i][j][0] = (ImagePosition){get_layer_img_func(i*4 + j), home_point.x, home_point.y};
            for (int k = 0; k < 6; k++) {
                lcd_layer_app_images[i][j][k + 1] = (ImagePosition){get_img_func(virtual_keycode[k + i*24 + j*6]), circles[k].x, circles[k].y};
            }
        }
    }
}

void draw_lcd_layer_category_images(void) {
    ImagePosition *images = lcd_layer_app_images[current_lcd_category][current_lcd_layer];
    int image_count = sizeof(lcd_layer_app_images[current_lcd_category][current_lcd_layer]) / sizeof(lcd_layer_app_images[current_lcd_category][current_lcd_layer][0]);

    for (int i = 0; i < image_count; i++) {
        if (*images[i].image != NULL) {
            int draw_x = images[i].x - (*images[i].image)->width / 2;
            int draw_y = images[i].y - (*images[i].image)->height / 2;
            qp_drawimage(display, draw_x, draw_y, *images[i].image);
        }
    }
    qp_flush(display);
}

void update_lcd_layer_category_by_gesture(void) {
    switch (gesture_id) {
        case CST816S_SLIDE_RIGHT:

            // 横軸を右にスライド
            if (current_lcd_layer < MAX_LCD_LAYER) {
                current_lcd_layer++;
                draw_background_all();
                draw_lcd_layer_category_images();
            } else {
                current_lcd_layer = MAX_LCD_LAYER; // ループする場合
            }

            break;

        case CST816S_SLIDE_LEFT:
            // 横軸を左にスライド
            if (current_lcd_layer > 0) {
                current_lcd_layer--;
                draw_background_all();
                draw_lcd_layer_category_images();
            } else {
                current_lcd_layer = 0; // ループする場合

            }

            break;

        case CST816S_SLIDE_UP:
            // 縦軸を上にスライド
            if (current_lcd_category > 0) {
                current_lcd_category--;
                draw_background_all();
                draw_lcd_layer_category_images();
            } else {
                current_lcd_category = 0; // ループする場合
            }

            break;

        case CST816S_SLIDE_DOWN:
            // 縦軸を下にスライド
            if (current_lcd_category < MAX_LCD_CATEGORY) {
                current_lcd_category++;
                draw_background_all();
                draw_lcd_layer_category_images();
            } else {
                current_lcd_category = MAX_LCD_CATEGORY; // ループする場合
            }

            break;

        default:
            break;
    }
    // レイヤーとカテゴリーに応じて画像を再描画

}

bool is_touch_in_circle(uint16_t touch_x, uint16_t touch_y, point_t circle, uint16_t radius) {
    uint32_t distance_squared = (circle.x - touch_x) * (circle.x - touch_x) + (circle.y - touch_y) * (circle.y - touch_y);
    return distance_squared <= (radius * radius);
}

void process_touch(uint16_t touch_x, uint16_t touch_y) {
    last_tap_time = timer_read();
    touch_signal = true;
    touch_signal_view_updata = true;
}

void process_gesture(cst816t_XY touch_data) {
    // uprintf("g id: =%d}/\n", gesture_id);
    if (gesture_id == CST816S_SLIDE_UP || gesture_id == CST816S_SLIDE_DOWN || gesture_id == CST816S_SLIDE_LEFT || gesture_id == CST816S_SLIDE_RIGHT) {
        update_lcd_layer_category_by_gesture(); // ジェスチャーによるレイヤー・カテゴリー切り替え
    }else if (gesture_id == CST816S_CLICK) {


        switch (display_mode) {
            case DISPLAY_MODE_TOUCH_KEY:
                process_touch(touch_x, touch_y);
                break;

            case DISPLAY_MODE_TRACKBALL_TUNING:
                if (timer_elapsed(last_touch_time) > TOUCH_DEBOUNCE_TIME) {
                    last_touch_time = timer_read();
                    process_touch_trackball_tuning_mode(touch_x, touch_y);
                }
                break;

            default:
                break;
        }
    } else if (gesture_id == CST816S_LONG_PRESS) {
        if (timer_elapsed(long_touch_time) > LONG_TOUCH_TIME) {
            switch_display_mode();
            long_touch_time = timer_read();
        }
    }

}


void process_touch_interrupt(void) {

    uint8_t interrupt_pin = readPin(INT_PIN);
    // uprintf("pin : =%d}/\n", interrupt_pin);
    // if (gesture_id != CST816S_LONG_PRESS) {
    //    long_touch_time = timer_read();
    // }

    if (interrupt_pin == 0) {
        single_click_flag = true;
        single_click_timer = timer_read();

        cst816t_XY touch_data = cst816t_Get_Point();
        touch_x = touch_data.x_point;
        touch_y = touch_data.y_point;

        single_touch_x = touch_x;
        single_touch_y = touch_y;
        gesture_id = touch_data.Gesture;
        if (gesture_id != 0) {
            single_click_flag = false;
        }

        process_gesture(touch_data);
        uprintf("g id: =%d}/\n", gesture_id);
        uprintf("corrdx: =%d}/\n", touch_data.x_point);

    } else {
    if (single_click_flag) {
        if (timer_elapsed(single_click_timer) > SINGLE_CLICK_TIME) {
            uprintf("add touch: =%d}/\n", gesture_id);
            touch_x = single_touch_x;
            touch_y = single_touch_y;

            uprintf("add corrrdx: =%d}/\n", touch_x);
            single_click_flag = false;
            last_tap_time = timer_read();

            switch (display_mode) {
                case DISPLAY_MODE_TOUCH_KEY:
                    process_touch(touch_x, touch_y);
                    break;

                case DISPLAY_MODE_TRACKBALL_TUNING:
                    if (timer_elapsed(last_touch_time) > TOUCH_DEBOUNCE_TIME) {
                        last_touch_time = timer_read();
                        process_touch_trackball_tuning_mode(touch_x, touch_y);
                    }
                    break;

                default:
                    break;
            }

        }
    }
    if (timer_elapsed(last_tap_time) > TOUCH_DEBOUNCE_TIME) {
            touch_signal = false; // タッチが解除されたので false に設定
        }
    }
}

void load_virtual_keys(void) {
    for (int row = 0; row < MATRIX_ROWS / 2; row++) {  // 行のループ
        for (int j = 0; j < MATRIX_COLS; j++) {  // 列のループ
            import_keymaps[row][j] = keymap_key_to_keycode(1, (keypos_t){.row = row, .col = j});
        }
    }
    int i = 0;
    for (int row = 0; row < MATRIX_ROWS / 2; row++) {  // 行のループ
        if (row == 0 || row == 1 || row == 2 || row == 3) {
            continue; // 該当する列は飛ばす
        }
        for (int col = 0; col < MATRIX_COLS; col++) {  // 列のループ
            virtual_keycode[i] = import_keymaps[row][col];
            i++;
        }
    }
}

// EEPROMにデータを保存する関数
void save_to_eeprom(void) {
    // グローバル変数の値を構造体にセット
    calibration_data.speed_adjust1 = (int)round(speed_adjust1 * 10);
    calibration_data.slope_factor1 = slope_factor1;
    calibration_data.speed_adjust2 = (int)round(speed_adjust2 * 10);
    calibration_data.slope_factor2 = slope_factor2;
    eeprom_update_block((void *)&calibration_data, (void *)EEPROM_OMNI_TB, sizeof(calibration_data));

}

// EEPROMからデータを読み込む関数
void load_from_eeprom(void) {
    // if (!eeconfig_is_enabled()) {
    //     eeconfig_init();  // EEPROMを初期化
    // }

    eeprom_read_block((void *)&calibration_data, (void *)EEPROM_OMNI_TB, sizeof(calibration_data));


    // uprintf("loading to EEPROM: speed_adjust1=%d, slope_factor1=%d, speed_adjust2=%d, slope_factor2=%d\n",
    //         calibration_data.speed_adjust1, calibration_data.slope_factor1,
    //         calibration_data.speed_adjust2, calibration_data.slope_factor2);
    // 読み込んだデータを個別の変数に反映
    speed_adjust1 = calibration_data.speed_adjust1 / 10.0f;
    slope_factor1 = calibration_data.slope_factor1;
    speed_adjust2 = calibration_data.speed_adjust2 / 10.0f;
    slope_factor2 = calibration_data.slope_factor2;
    if (speed_adjust1 > 3.0f || speed_adjust1 < 0.1f) {
        speed_adjust1 = DEFAULT_SPEED_ADJUST1;
    }
    if (slope_factor1 > 100 || slope_factor1 < 10) {
        slope_factor1 = DEFAULT_SLOPE_FACTOR1;
    }
    if (speed_adjust2 > 3.0f || speed_adjust2 < 0.1f) {
        speed_adjust2 = DEFAULT_SPEED_ADJUST2;
    }
    if (slope_factor2 > 100 || slope_factor2 < 10) {
        slope_factor2= DEFAULT_SLOPE_FACTOR2;
    }

}


void show_trackball_tuning_mode(void) {
    draw_background_all();

    if (text1_width == 0) {
        text1_width = qp_textwidth(noto11_font, text1);
    }
    if (text3_width == 0) {
        text3_width = qp_textwidth(noto11_font, text3);
    }
    if (text4_width == 0) {
        text4_width = qp_textwidth(noto11_font, text4);
    }
    if (text5_width == 0) {
        text5_width = qp_textwidth(noto11_font, text5);
    }
    if (text6_width == 0) {
        text6_width = qp_textwidth(noto11_font, text6);
    }

    qp_drawtext(display, 120 - text1_width/2, 50 - noto11_font->line_height, noto11_font, text1);
    qp_drawtext(display, x_pos1 - text3_width/2, 200 - noto11_font->line_height, noto11_font, text3);
    qp_drawtext(display, x_pos1 - text5_width/2, y_pos3 - noto11_font->line_height /2, noto11_font, text5);
    qp_drawtext(display, x_pos1 - text6_width/2, y_pos4 - noto11_font->line_height /2, noto11_font, text6);
    qp_drawtext(display, x_pos2 - text4_width/2, 200 - noto11_font->line_height, noto11_font, text4);
    qp_drawtext(display, x_pos2 - text5_width/2, y_pos3 - noto11_font->line_height /2, noto11_font, text5);
    qp_drawtext(display, x_pos2 - text6_width/2, y_pos4 - noto11_font->line_height /2, noto11_font, text6);
    qp_curve(display, speed_adjust1, slope_factor1, cpi, hue1, sat1, val1, x_pos1, y_pos1);
    qp_curve(display, speed_adjust2, slope_factor2, cpi, hue2, sat2, val2, x_pos2, y_pos2);
    qp_drawimage(display, 120 - image_save->width/2, 210 - image_save->height/2, image_save);

    // qp_circle(display, x_pos1+25+1, y_pos3, 22, 220, 255, 255, false);
    // qp_circle(display, x_pos1-25-1, y_pos3, 22, 220, 255, 255, false);
    // qp_circle(display, x_pos2+25+1, y_pos3, 22, 220, 255, 255, false);
    // qp_circle(display, 120, 210, 22, 220, 255, 255, false);

    qp_flush(display);
}
// ディスプレイモードを設定する関数
void switch_display_mode(void) {
    switch (display_mode) {
        case DISPLAY_MODE_TOUCH_KEY:
            display_mode = DISPLAY_MODE_TRACKBALL_TUNING;
            show_trackball_tuning_mode();
            break;

        case DISPLAY_MODE_TRACKBALL_TUNING:
            display_mode = DISPLAY_MODE_TOUCH_KEY;
            draw_background_all();
            draw_lcd_layer_category_images();
            break;

        default:
            break;
    }
}

void process_touch_trackball_tuning_mode(uint16_t touch_x, uint16_t touch_y) {
    // 判定範囲の設定 (+/-それぞれに対応)
    const uint8_t radius = 22;  // タッチ判定の許容半径
    const uint8_t radius_gap = 1;
    point_t plus1_center = {x_pos1 - (radius + radius_gap), y_pos4};  // x_pos1の+ボタンの中心
    point_t minus1_center = {x_pos1 - (radius + radius_gap), y_pos3}; // x_pos1の-ボタンの中心
    point_t plus2_center = {x_pos2 - (radius + radius_gap), y_pos4};  // x_pos2の+ボタンの中心
    point_t minus2_center = {x_pos2 - (radius + radius_gap), y_pos3}; // x_pos2の-ボタンの中心

    point_t plus3_center = {x_pos1 + (radius + radius_gap), y_pos3};  // x_pos1の+ボタンの中心
    point_t minus3_center = {x_pos1 + (radius + radius_gap), y_pos4}; // x_pos1の-ボタンの中心
    point_t plus4_center = {x_pos2 + (radius + radius_gap), y_pos3};  // x_pos2の+ボタンの中心
    point_t minus4_center = {x_pos2 + (radius + radius_gap), y_pos4}; // x_pos2の-ボタンの中心

    point_t save_center = {120, 215}; // saveボタンの中心
    // point_t load_center = {120, 120}; // saveボタンの中心

    // x_pos1の+ボタンがタッチされた場合
    if (is_touch_in_circle(touch_x, touch_y, plus1_center, radius)) {
        speed_adjust1 += 0.2;  // speed_adjust1を加算
        if (speed_adjust1 > 3.0f) speed_adjust1 = 3.0f;  // 最大値を制限
        // qp_dprintf("Touched +1: speed_adjust1 = %f\n", speed_adjust1);
    }
    // x_pos1の-ボタンがタッチされた場合
    else if (is_touch_in_circle(touch_x, touch_y, minus1_center, radius)) {
        speed_adjust1 -= 0.2;  // speed_adjust1を減算
        if (speed_adjust1 < 0.2f) speed_adjust1 =  0.2f;  // 最小値を制限
        // qp_dprintf("Touched -1: speed_adjust1 = %f\n", speed_adjust1);
    }
    // x_pos2の+ボタンがタッチされた場合
    else if (is_touch_in_circle(touch_x, touch_y, plus2_center, radius)) {
        speed_adjust2 += 0.2;  // speed_adjust2を加算
        if (speed_adjust2 > 3.0f) speed_adjust2 = 3.0f;  // 最大値を制限
        // qp_dprintf("Touched +2: speed_adjust2 = %f\n", speed_adjust2);
    }
    // x_pos2の-ボタンがタッチされた場合
    else if (is_touch_in_circle(touch_x, touch_y, minus2_center, radius)) {
        speed_adjust2 -= 0.2;  // speed_adjust2を減算
        if (speed_adjust2 < 0.2f) speed_adjust2 = 0.2f;  // 最小値を制限
        // qp_dprintf("Touched -2: speed_adjust2 = %f\n", speed_adjust2);
    }

    // x_pos2の+ボタンがタッチされた場合
    else if (is_touch_in_circle(touch_x, touch_y, plus3_center, radius)) {
        slope_factor1 += 5;  //
        if (slope_factor1 > 100) slope_factor1 = 100;  // 最大値を制限
        // qp_dprintf("Touched +2: slope_factor1 = %f\n", slope_factor1);
    }
    // x_pos2の-ボタンがタッチされた場合
    else if (is_touch_in_circle(touch_x, touch_y, minus3_center, radius)) {
        slope_factor1 -= 5;  //
        if (slope_factor1 < 10) slope_factor1 = 10;  // 最小値を制限
        // qp_dprintf("Touched -2: slope_factor1 = %f\n", slope_factor1);
    }
    // x_pos2の+ボタンがタッチされた場合
    else if (is_touch_in_circle(touch_x, touch_y, plus4_center, radius)) {
        slope_factor2 += 5;  //
        if (slope_factor2 > 100) slope_factor2 = 100;  // 最大値を制限
        // qp_dprintf("Touched +2: slope_factor2 = %f\n", slope_factor2);
    }
    // x_pos2の-ボタンがタッチされた場合
    else if (is_touch_in_circle(touch_x, touch_y, minus4_center, radius)) {
        slope_factor2 -= 5;  // slope_factor2を減算
        if (slope_factor2 < 10) slope_factor2 = 10;  // 最小値を制限
        // qp_dprintf("Touched -2: slope_factor2 = %f\n", slope_factor2);
    }
    // saveボタンがタッチされた場合
    else if (is_touch_in_circle(touch_x, touch_y, save_center, radius)) {
        save_to_eeprom(); // `save`ボタン押下時にEEPROMに保存
        show_trackball_tuning_mode();  // 画面を再描画
        // qp_dprintf("Calibration data saved to EEPROM.\n");
    }

//デバッグ用
        // else if (is_touch_in_circle(touch_x, touch_y, load_center, radius)) {
        //     load_from_eeprom(); // 中央を押下時にEEPROMをload
        //     show_trackball_tuning_mode();  // 画面を再描画
        // }




    if (pre_speed_adjust1 != speed_adjust1 || pre_slope_factor1 != slope_factor1) {
        draw_background(135, 80, 205, 145);
        wait_ms(10);
        qp_curve(display, speed_adjust1, slope_factor1, cpi, hue1, sat1, val1, x_pos1, y_pos1);
    }else if (pre_speed_adjust2 != speed_adjust2 || pre_slope_factor2 != slope_factor2) {
        draw_background(35, 80, 105, 145);
        wait_ms(10);
        qp_curve(display, speed_adjust2, slope_factor2, cpi, hue2, sat2, val2, x_pos2, y_pos2);
    }

    pre_speed_adjust1 = speed_adjust1;
    pre_speed_adjust2 = speed_adjust2;
    pre_slope_factor1 = slope_factor1;
    pre_slope_factor2 = slope_factor2;
}

bool is_center_touch(uint16_t touch_x, uint16_t touch_y) {
    const uint16_t center_x = 120, center_y = 120, radius = 30; // 中心座標と許容半径
    uint32_t distance_squared = (touch_x - center_x) * (touch_x - center_x) +
                                (touch_y - center_y) * (touch_y - center_y);
    return distance_squared <= (radius * radius);
}

void update_lcd_view_data(void){
    if (memcmp(virtual_keycode, pre_virtual_keycode, KEYCODE_SIZE * sizeof(uint16_t)) != 0) {
        memcpy(pre_virtual_keycode, virtual_keycode, KEYCODE_SIZE * sizeof(uint16_t));
        draw_background_all();
        initialize_lcd_layer_app_images();
        draw_lcd_layer_category_images();
    }
}

int8_t is_usb_connected(void) {
    usbstate_t state = usbGetDriverStateI(&USB_DRIVER);
    // uprintf("USB State: %d\n", state);  // USBの状態を出力
    return state;
}

bool power_on_lcd(void) {
    qp_power(display, true);
    wait_ms(10);
    setPinOutput(BLK_PIN);
    writePinHigh(BLK_PIN);
    return  true;
}

bool power_off_lcd(void) {
    setPinOutput(BLK_PIN);
    writePinLow(BLK_PIN);
    wait_ms(1000);
    qp_power(display, false);
    return false;
}

void manage_lcd_power(int8_t usb_state) {
    if (lcd_is_on) {
        if (usb_state == USB_UNINIT || usb_state == USB_STOP) {
            lcd_is_on = power_off_lcd();
        }
    } else {
        if (usb_state == USB_ACTIVE) {
            lcd_is_on = power_on_lcd();
        }
    }
}

void pointing_device_init_kb(void) {
    pmw33xx_init(0);         // index 1 is the fast device.
    pmw33xx_init(1);         // index 1 is the second device.a
    pmw33xx_set_cpi(0, 4000); // applies to first sensor
    pmw33xx_set_cpi(1, 10000); // applies to second sensor
    pointing_device_init_user();
}

void keyboard_post_init_kb(void) {
    load_from_eeprom(); // EEPROMからデータを読み込む
    // GC9A01ディスプレイを作成
    display = qp_gc9a01_make_spi_device(240, 240, CS_PIN, DC_PIN, RST_PIN, 4, 0); // パネル幅, パネル高さ,,,,SPIディバイザ,SPIモード

    // ディスプレイの初期化
    qp_init(display, QP_ROTATION_0);   // 0度回転で初期化
    noto11_font = qp_load_font_mem(font_noto11);  // `font_noto11` を使用

    // 2: ミックスモード
    // 0: ジェスチャーモード
    // 1: ポイントモード
    uint8_t init_status = cst816t_init(2);
    if (init_status == 0) {
        uprintf("Touch initialization failed.\n");
    } else {
        uprintf("Touch initialization successful.\n");
    }

    // 初回フレームでのみ背景を描画
    draw_background_all();
    initialize_images();

    qp_flush(display);  // ディスプレイに描画内容を反映
    if (image_logo != NULL) {
        int x = (240 - image_logo->width) / 2;  // 画面中央に配置するためのX座標
        int y = (240 - image_logo->height) / 2; // 画面中央に配置するためのY座標
        my_anim = qp_animate(display, x, y, image_logo);  // 画像を描画
        lcd_fast_res_time = timer_read();
    }
}

void matrix_init_user(void) {
    setPinOutput(BLK_PIN);
    writePinHigh(BLK_PIN);  // バックライトをONにする
    // draw_background_all();
    i2c_init();  // I2C初期化
    setPinInputHigh(INT_PIN);  // タッチ割り込みピンの設定
    initialize_lcd_layer_app_images();
}

static bool tb_state = false;

report_mouse_t  pointing_device_task_kb(report_mouse_t mouse_report) {
    pmw33xx_report_t report0 = pmw33xx_read_burst(0); // Sensor #1
    pmw33xx_report_t report1 = pmw33xx_read_burst(1); // Sensor #2

    if (report0.motion.b.is_motion || report1.motion.b.is_motion) {
        tb_state = true;
    }else {
        tb_state = false;
    }

    process_cursor_report(&mouse_report, report0, speed_adjust1, slope_factor1);
    process_tap_report(&mouse_report, report1, speed_adjust2, slope_factor2);

    if (ENABLE_TOUCH_UPFDATE == 1) {
        if (touch_signal_view_updata) {
            // バックライトのオン/オフを管理
            if (!is_backlight_off) {
                // setPinOutput(BLK_PIN);
                writePinLow(BLK_PIN);  // バックライトをオフ
                blink_start_time = timer_read();  // オフにした時刻を記録
                is_backlight_off = true;
            }

            // 一定時間（50ms）が経過したらバックライトをオンに戻す
            if (is_backlight_off && timer_elapsed(blink_start_time) >= 50) {
                writePinHigh(BLK_PIN);  // バックライトをオン
                is_backlight_off = false;
                touch_signal_view_updata = false;  // フラグをリセット
            }
        }
    }

    if (!is_first_frame) {
        if (is_second_frame) {
            draw_lcd_layer_category_images();  // 現在のLCDレイヤーとカテゴリーの画像を描画
            qp_flush(display);
            is_second_frame = false;
        }
    }



    return pointing_device_task_user(mouse_report);
}

void matrix_scan_user(void) {
    process_touch_interrupt();
    load_virtual_keys();
    update_lcd_view_data();
}



void sleeping_kb(bool matrix_changed) {
     if (matrix_changed || tb_state || single_click_flag) {
        sleeping_timer = timer_read();
        if (sleeping_state) {
            lcd_is_on = power_on_lcd();
            sleeping_state = false;
        }
    }
    if (!sleeping_state) {
        if (timer_elapsed(sleeping_timer) >= SLEEPING_KB_TIME) {
            lcd_is_on = power_off_lcd();
            sleeping_state = true;
        }
    }
}


void housekeeping_task_user(void) {
    lcd_current_time = timer_read();
    if (lcd_current_time - lcd_fast_res_time > 3000) {
        if (is_first_frame) {
            is_first_frame = false;  // 一度描画したら次回からはスキップ
            qp_stop_animation(my_anim);
            draw_background_all();
            initialize_lcd_layer_app_images();
        }
    }

    if (lcd_current_time - last_update > update_interval) {
        // 更新が必要な部分だけ再描画
        // qp_line(display, 50, 50, 100, 0, 0, 100, 100);
        // qp_flush(display);
        last_update = lcd_current_time;
    }

    // if (timer_elapsed(last_usb_check_time) >= 500) {
    //     last_usb_check_time = timer_read();  // タイマーをリセット
    //     int8_t usb_state = is_usb_connected();
    //     manage_lcd_power(usb_state);
    // }

    matrix_changed = get_last_matrix_state();
    // if (matrix_changed) {
    //     uprintf("Matrix state changed!\n");
    // }
    sleeping_kb(matrix_changed);

}



