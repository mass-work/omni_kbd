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
// #include "eeprom.h"
#include "usb_main.h"
#include "usb_driver.h"
#include "matrix.h"
#include "config.h"
#include "omni_cs.h"
#include "dynamic_keymap.h"

#include "../drivers/pmw33xx_common.h"
#include "../drivers/cst816t.h"
#include "../customfunc/sleeping_view.h"
#include "../customfunc/draw_custom.h"
#include "../customfunc/get_custom_gesture.h"
#include "../customfunc/view_keymap.h"
#include "../font/noto11.qff.h"
#include "../font/roboto_mono16.qff.h"
#include "../font/st2_mono16.qff.h"
#include "../icon/omni_image_loader.h"

#define MACRO_KEY_START 0x7700
#define MACRO_KEY_END   0x77FE
#define MACRO_KEY_COUNT (MACRO_KEY_END - MACRO_KEY_START + 1)
#define constrain_hid(amt) ((amt) < -127 ? -127 : ((amt) > 127 ? 127 : (amt)))

uint8_t current_layer = 0;

uint16_t draw_matrix_code_rain_timer = 0;
bool fast_draw_matrix_code_rain = false;
bool fast_draw_radar_view = false;
uint16_t draw_radar_view_timer = 0;

uint8_t hue_bg = 0;
uint8_t sat_bg = 0;
uint8_t val_bg = 0;
uint8_t hue_main_color = 255;
uint8_t sat_main_color = 0;
uint8_t val_main_color = 255;
uint8_t hue_sub_color = 255;
uint8_t sat_sub_color = 0;
uint8_t val_sub_color = 255;

uint32_t sleeping_timer;
bool matrix_changed = false;
bool sleeping_state = false;

static float accumulated_x = 0.0f;
static float accumulated_y = 0.0f;
static float accumulated_h = 0.0f;
static float accumulated_v = 0.0f;
static bool tb_state = false;
static uint8_t pre_layer = 0; 
trackball_mode_t tb_mode_r = TRACKBALL_CURSOR;
trackball_mode_t tb_mode_l = TRACKBALL_TAP;

static painter_device_t display;
painter_font_handle_t noto11_font;
painter_font_handle_t roboto_mono16;
painter_font_handle_t st2_mono16;
painter_image_handle_t qp_load_image_mem(const void *buffer);
painter_image_handle_t image_logo;
painter_image_handle_t image_save;
painter_image_handle_t layer_00, layer_01, layer_02, layer_03, layer_04, layer_05, layer_06, layer_07, layer_08, layer_09, layer_10, layer_11;
painter_image_handle_t image_000, image_001, image_002, image_003, image_004, image_005, image_006, image_007, image_008, image_009, image_010, image_011, image_012, image_013, image_014, image_015, image_016, image_017, image_018, image_019, image_020, image_021, image_022, image_023, image_024, image_025, image_026, image_027, image_028, image_029, image_030, image_031, image_032, image_033, image_034, image_035, image_036, image_037, image_038, image_039, image_040, image_041, image_042, image_043, image_044, image_045, image_046, image_047, image_048, image_049, image_050, image_051, image_052, image_053, image_054, image_055, image_056, image_057, image_058, image_059, image_060, image_061, image_062, image_063, image_064, image_065, image_066, image_067, image_068, image_069, image_070, image_071, image_072, image_073, image_074, image_075, image_076, image_077, image_078, image_079, image_080, image_081, image_082, image_083, image_084, image_085, image_086, image_087, image_088, image_089, image_090, image_091, image_092, image_093, image_094, image_095, image_096, image_097, image_098, image_099, image_100, image_101, image_102, image_103, image_104, image_105, image_106, image_107, image_108, image_109, image_110, image_111, image_112, image_113, image_114, image_115, image_116, image_117, image_118, image_119, image_120, image_121, image_122, image_123, image_124, image_125, image_126, image_127, image_128, image_129, image_130, image_131, image_132, image_133, image_134, image_135, image_136, image_137, image_138, image_139, image_140, image_141, image_142, image_143, image_144, image_145, image_146, image_147, image_148, image_149, image_150, image_151, image_152, image_153, image_154, image_155, image_156, image_157, image_158, image_159, image_160, image_161, image_162, image_163, image_164, image_165, image_166, image_167, image_168, image_169, image_170, image_171, image_172, image_173, image_174, image_175, image_176, image_177, image_178, image_179, image_180, image_181, image_182, image_183, image_184, image_185, image_186, image_187, image_188, image_189, image_190, image_191, image_192, image_193, image_194, image_195, image_196, image_197, image_198, image_199, image_200, image_201, image_202, image_203, image_204, image_205, image_206, image_207, image_208, image_209, image_210, image_211, image_212, image_213, image_214, image_215, image_216, image_217, image_218, image_219, image_220, image_221, image_222, image_223, image_224, image_225, image_226, image_227, image_228, image_229, image_230, image_231, image_232, image_233, image_234, image_235, image_236, image_237, image_238, image_239, image_240, image_241, image_242, image_243, image_244, image_245, image_246, image_247, image_248, image_249, image_250, image_251, image_252, image_253, image_254;
bool is_first_frame = true;  
bool is_second_frame = true;  
static deferred_token my_anim;
const uint32_t update_interval = 50;  
uint32_t lcd_current_time = 0;
uint32_t lcd_fast_res_time = 0;
uint8_t touch_data[6];
ImagePosition images[7];
ImagePosition lcd_layer_app_images[MAX_LCD_CATEGORY + 1][MAX_LCD_LAYER + 1][7];
uint8_t current_lcd_layer = 0;
uint8_t current_lcd_category = 0;
point_t home_point = {120, 120};
point_t circles[] = {{200, 120}, {160, 189}, {80, 189}, {40, 120}, {80, 51}, {160, 51}};
uint8_t gesture_id = GESTURE_NONE;
uint16_t touch_x = 0xFFFF;
uint16_t touch_y = 0xFFFF;
void process_touch_trackball_tuning_mode(uint16_t touch_x, uint16_t touch_y);
static uint16_t last_touch_time = 0;
uint16_t current_time = 0;
static uint16_t last_tap_time = 0;
bool touch_signal = false;
bool touch_signal_view_update = false;
uint16_t virtual_keycode[KEYCODE_SIZE];
uint16_t pre_virtual_keycode[KEYCODE_SIZE];
uint16_t import_keymaps[MATRIX_ROWS / 2][MATRIX_COLS];

float speed_adjust1;
float pre_speed_adjust1;
int slope_factor1;
int pre_slope_factor1;
uint8_t hue_tbtune_r = 220;
uint8_t sat_tbtune_r = 255;
uint8_t val_tbtune_r = 255;
uint8_t x_pos1 = 170;
uint8_t y_pos1 = 110;

float speed_adjust2;
float pre_speed_adjust2;
int slope_factor2;
int pre_slope_factor2;
uint8_t hue_tbtune_l = 135;
uint8_t sat_tbtune_l = 255;
uint8_t val_tbtune_l = 255;
uint8_t x_pos2 = 70;
uint8_t y_pos2 = 110;

uint8_t y_pos3 = 70;
uint8_t y_pos4 = 165;
static int16_t text1_width, text3_width, text4_width, text5_width, text6_width = 0;

display_mode_t display_mode =  DISPLAY_MODE_TOUCH_KEY;
bool enable_touch_update = true;

static uint32_t blink_start_time = 0;
static bool is_backlight_off = false;
bool lcd_is_on = true;
uint32_t last_usb_check_time = 0;
uint16_t pre_touch_x;
uint16_t pre_touch_y;

static const char *text1 = "TB TUNE";
static const char *text3 = "Right";
static const char *text4 = "Left";
static const char *text5 = "+      +";
static const char *text6 = "-      -";

int8_t ud_sc_mode_flag = 1;
int8_t lr_sc_mode_flag = 1;

void process_cursor_report(report_mouse_t *mouse_report, pmw33xx_report_t report, float speed_adjust, uint8_t slope_factor, int rx, int ry, uint8_t cpi_scale) {
    if (!report.motion.b.is_lifted) {
        float x = (report.delta_x / cpi_scale);
        float y = (report.delta_y / cpi_scale);
        int sign_x = ((x > 0) - (x < 0)) * rx;
        int sign_y = ((y > 0) - (y < 0)) * ry;
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

void process_tap_report(report_mouse_t *mouse_report, pmw33xx_report_t report, float speed_adjust, uint8_t slope_factor, int rx, int ry, uint8_t cpi_scale) {
    if (!report.motion.b.is_lifted) {
        int x = (report.delta_x / cpi_scale);
        int y = (report.delta_y / cpi_scale);
        int sign_x = ((x > 0) - (x < 0)) * rx * lr_sc_mode_flag;
        int sign_y = ((y > 0) - (y < 0)) * ry * ud_sc_mode_flag;
        float x_corr = pow(fabs(x), speed_adjust) / pow(127, speed_adjust) * 127 / 100 * slope_factor * sign_x;
        float y_corr = pow(fabs(y), speed_adjust) / pow(127, speed_adjust) * 127 / 100 * slope_factor * sign_y;
        const float diagonal_limit = 0.6f;
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
            accumulated_h = 0.0f;
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
    return &image_001;
}

void draw_background(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
    qp_rect(display, x1, y1, x2, y2, hue_bg, sat_bg, val_bg, true);  // HSV: H=0, S=0, V=0 (黒色)
}

void draw_background_black(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
    qp_rect(display, x1, y1, x2, y2, 0, 0, 0, true);  // HSV: H=0, S=0, V=0 (黒色)
}

void draw_background_all(void) {
    qp_rect(display, 0, 0, 240, 240, hue_bg, sat_bg, val_bg, true);  // HSV: H=0, S=0, V=0 (黒色)
}

void draw_background_all_black(void) {
    qp_rect(display, 0, 0, 240, 240, 0, 0, 0, true);  // HSV: H=0, S=0, V=0 (黒色)
}

void initialize_lcd_layer_app_images(void) {
    for (int j = 0; j < 4; j++) {
        for (int i = 0; i < 3; i++) {
            lcd_layer_app_images[i][j][0] = (ImagePosition){get_layer_img_func(i + j*3), home_point.x, home_point.y};
            for (int k = 0; k < 6; k++) {
                lcd_layer_app_images[i][j][k + 1] = (ImagePosition){get_img_func(virtual_keycode[k + i*6 + j*18]), circles[k].x, circles[k].y};
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
            if (current_lcd_layer < MAX_LCD_LAYER) {
                current_lcd_layer++;
                draw_background_all_black();
                draw_lcd_layer_category_images();
            } else {
                current_lcd_layer = MAX_LCD_LAYER; 
            }
            break;

        case CST816S_SLIDE_LEFT:
            if (current_lcd_layer > 0) {
                current_lcd_layer--;
                draw_background_all_black();
                draw_lcd_layer_category_images();
            } else {
                current_lcd_layer = 0; 
            }
            break;

        case CST816S_SLIDE_UP:
            if (current_lcd_category > 0) {
                current_lcd_category--;
                draw_background_all_black();
                draw_lcd_layer_category_images();
            } else {
                current_lcd_category = 0;
            }
            break;

        case CST816S_SLIDE_DOWN:
            if (current_lcd_category < MAX_LCD_CATEGORY) {
                current_lcd_category++;
                draw_background_all_black();
                draw_lcd_layer_category_images();
            } else {
                current_lcd_category = MAX_LCD_CATEGORY; 
            }
            break;

        default:
            break;
    }
}
    
int swipe_layer = 0;
void change_swipe_layer(void) {
    swipe_layer = (swipe_layer + 1) % 4;
}

const char* get_layer_name(uint8_t layer) {
    switch (layer) {
        case 0:
            return "BASE";
        case 1:
            return "NUM ";
        case 2:
            return "SYMB";
        case 3:
            return "CUST";
        default:
            return "UNKN";
    }
}

void swipe_gesture_main_view_update(uint8_t current_layer) {
    const char *layer_text = get_layer_name(current_layer);
    uint16_t layer_text_width = qp_textwidth(noto11_font, layer_text);
    draw_background(85, 110, 155, 130); 
    qp_drawtext_recolor(display, 120 - layer_text_width / 2, 120 - noto11_font->line_height / 2, noto11_font, layer_text, hue_main_color, sat_main_color, val_main_color, hue_bg, sat_bg, val_bg);
    qp_flush(display);  
} 

void swipe_gesture_base_view_update(void) {
    qp_donut(display, 120, 120, 117, 4, hue_main_color, sat_main_color, 50, val_main_color);
    qp_donut(display, 120, 120, 114, 4, hue_main_color, sat_main_color, val_main_color, 50);
    qp_circle(display, 90 , 190, 5, hue_sub_color, sat_sub_color, val_sub_color, false);
    qp_circle(display, 110, 190, 5, hue_sub_color, sat_sub_color, val_sub_color, false);
    qp_circle(display, 130, 190, 5, hue_sub_color, sat_sub_color, val_sub_color, false);
    qp_circle(display, 150, 190, 5, hue_sub_color, sat_sub_color, val_sub_color, false);
}

void swipe_gesture_view_update(char *text_r, char *text_l, char *text_u, uint8_t x1, uint8_t x2) {
    uint8_t text_width_r = qp_textwidth(noto11_font, text_r);
    uint8_t text_width_L = qp_textwidth(noto11_font, text_l);
    uint8_t text_width_U = qp_textwidth(noto11_font, text_u);
    uint8_t text_hight = noto11_font->line_height;
    draw_background(150, 120 - text_hight / 2, 230, 120 + text_hight / 2);  
    draw_background(10 , 120 - text_hight / 2, 90 , 120 + text_hight / 2); 
    draw_background(80 , 40  - text_hight / 2, 160, 40  + text_hight / 2); 
    qp_drawtext_recolor(display, 190 - text_width_r / 2, 120 - text_hight / 2, noto11_font, text_r, hue_main_color, sat_main_color, val_main_color, hue_bg, sat_bg, val_bg);
    qp_drawtext_recolor(display, 50  - text_width_L / 2, 120 - text_hight / 2, noto11_font, text_l, hue_main_color, sat_main_color, val_main_color, hue_bg, sat_bg, val_bg);
    qp_drawtext_recolor(display, 120 - text_width_U / 2, 40  - text_hight / 2, noto11_font, text_u, hue_main_color, sat_main_color, val_main_color, hue_bg, sat_bg, val_bg);
    qp_circle(display, x1, 190, 5, hue_sub_color, sat_sub_color, val_sub_color, true);
    qp_circle(display, x2, 190, 4, hue_bg, sat_bg, val_bg, true);
    qp_flush(display);
}

void swipe_gesture_layer_view_update(void){
    switch (swipe_layer) {
        case 0:
            swipe_gesture_view_update("NEXT", "BACK", "Save", 90, 150);
            break;
        case 1:
            swipe_gesture_view_update("PG>", "<PG", "NEW", 110, 90);
            break;  
        case 2:
            swipe_gesture_view_update("VD>", "<VD", "VDNEW", 130, 110);
            break;
        case 3:
            swipe_gesture_view_update("CST1", "CST2", "CST3", 150, 130);
            break;
        default:
            break;
    }
}

void swipe_gesture_process(void) {
        switch (swipe_layer) {
            case 0:
                switch (gesture_id) {
                    case CST816S_SLIDE_RIGHT:
                        register_code(KC_LCTL);
                        tap_code(KC_Y);
                        unregister_code(KC_LCTL);
                        break;
                    case CST816S_SLIDE_LEFT:
                        register_code(KC_LCTL);
                        tap_code(KC_Z);
                        unregister_code(KC_LCTL);
                        break;
                    case CST816S_SLIDE_UP:
                        register_code(KC_LCTL);
                        tap_code(KC_S);
                        unregister_code(KC_LCTL);
                        break;
                    case CST816S_SLIDE_DOWN:
                        change_swipe_layer();
                        swipe_gesture_layer_view_update();
                        break;
                    default:
                        break;
                }
                break;
            case 1:
                switch (gesture_id) {
                    case CST816S_SLIDE_RIGHT:
                        register_code(KC_LCTL);
                        tap_code(KC_PGDN);
                        unregister_code(KC_LCTL);
                        break;
                    case CST816S_SLIDE_LEFT:
                        register_code(KC_LCTL);
                        tap_code(KC_PGUP);
                        unregister_code(KC_LCTL);
                        break;
                    case CST816S_SLIDE_UP:
                        register_code(KC_LCTL);
                        tap_code(KC_N);
                        unregister_code(KC_LCTL);
                        break;
                    case CST816S_SLIDE_DOWN:
                        change_swipe_layer();
                        swipe_gesture_layer_view_update();
                        break;
                    default:
                        break;
                }
                break;
            case 2:
                switch (gesture_id) {
                    case CST816S_SLIDE_RIGHT:
                        register_code(KC_LCTL);
                        register_code(KC_LGUI);
                        tap_code(KC_RIGHT);
                        unregister_code(KC_LGUI);
                        unregister_code(KC_LCTL);
                        break;
                    case CST816S_SLIDE_LEFT:
                        register_code(KC_LCTL);
                        register_code(KC_LGUI);
                        tap_code(KC_LEFT);
                        unregister_code(KC_LGUI);
                        unregister_code(KC_LCTL);
                        break;
                    case CST816S_SLIDE_UP:
                        register_code(KC_LCTL);
                        register_code(KC_LGUI);
                        tap_code(KC_D);
                        unregister_code(KC_LGUI);
                        unregister_code(KC_LCTL);
                        break;
                    case CST816S_SLIDE_DOWN:
                        change_swipe_layer();
                        swipe_gesture_layer_view_update();
                        break;
                    default:
                        break;
                }
                break;
            case 3:
                switch (gesture_id) {
                    case CST816S_SLIDE_RIGHT:
                        tap_code(KC_F13);
                        break;
                    case CST816S_SLIDE_LEFT:
                        tap_code(KC_F14);
                        break;
                    case CST816S_SLIDE_UP:
                        tap_code(KC_F15);
                        break;
                    case CST816S_SLIDE_DOWN:
                        change_swipe_layer();
                        swipe_gesture_layer_view_update();
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
    // }

}

bool is_touch_in_circle(uint16_t touch_x, uint16_t touch_y, point_t circle, uint16_t radius) {
    uint32_t distance_squared = (circle.x - touch_x) * (circle.x - touch_x) + (circle.y - touch_y) * (circle.y - touch_y);
    return distance_squared <= (radius * radius);
}

void process_touch(uint16_t touch_x, uint16_t touch_y) {
    last_tap_time = timer_read();
    touch_signal = true;
    touch_signal_view_update = true;
}

uint16_t touch_start_timer = 0;
bool touch_start_flag = false; 

uint8_t classify_gesture(uint16_t x_start, uint16_t y_start, uint16_t x_last,  uint16_t y_last){
    if (gesture_id == CST816S_SLIDE_LEFT || gesture_id == CST816S_SLIDE_RIGHT || gesture_id == CST816S_SLIDE_UP || gesture_id == CST816S_SLIDE_DOWN) {
        return gesture_id;
    }
    uint16_t dx = (x_last > x_start) ? (x_last - x_start) : (x_start - x_last);
    uint16_t dy = (y_last > y_start) ? (y_last - y_start) : (y_start - y_last);

    if (dx < TAP_DIST_PX && dy < TAP_DIST_PX) {
        return CST816S_CLICK;
    }
    
    if (dx > dy && dx > SWIPE_DIST_PX) {
        return (x_last > x_start) ? CST816S_SLIDE_LEFT : CST816S_SLIDE_RIGHT;
    }
        if (dy > SWIPE_DIST_PX) {
        return (y_last > y_start) ? CST816S_SLIDE_UP : CST816S_SLIDE_DOWN;
    }

    return GESTURE_NONE;
}

void process_gesture(cst816t_XY touch_data) {
    // uprintf("x,y: %d, %d\n", touch_data.x_point, touch_data.y_point); 

    if (gesture_id == CST816S_SLIDE_UP || gesture_id == CST816S_SLIDE_DOWN || gesture_id == CST816S_SLIDE_LEFT || gesture_id == CST816S_SLIDE_RIGHT) {
        if (display_mode == DISPLAY_MODE_TOUCH_KEY) {
            update_lcd_layer_category_by_gesture();
        } else if (display_mode == DISPLAY_MODE_SWIPE_GESTURE) {
            swipe_gesture_process();
        }
    } else if (gesture_id == CST816S_CLICK) {
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


static bool finger_prev = false;
static bool finger_prev_flag = false;
uint16_t finger_prev_timer = 0;

void process_touch_interrupt(void) {
    uint8_t interrupt_pin = readPin(INT_PIN);
    if (finger_prev_flag) {
        if (timer_elapsed(finger_prev_timer) > 70) {
            finger_prev = get_finger_present(finger_prev);
            finger_prev_flag = false;
            if (finger_prev + gesture_id == 0) {
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
            // uprintf("finger_prev: %d\n", finger_prev);
            // uprintf("gesture: %d\n", gesture_id);
        }
    }
    
    if (interrupt_pin == 0) {
        finger_prev_timer = timer_read();
        finger_prev_flag = true;
        finger_prev = get_finger_present(finger_prev);
        
        if(!touch_start_flag) {
            touch_start_timer = timer_read();
            touch_start_flag = true;
            cst816t_XY touch_data = cst816t_Get_Point();
            pre_touch_x = touch_data.x_point;
            pre_touch_y = touch_data.y_point;
            // uprintf("g id: %d\n", pre_touch_y); 
            return;
        }

        if (timer_elapsed(touch_start_timer) >= TOUCH_TIME_MS) {
            cst816t_XY touch_data = cst816t_Get_Point(); 
            touch_x = touch_data.x_point;
            touch_y = touch_data.y_point;
            gesture_id = classify_gesture(touch_x, touch_y, pre_touch_x,  pre_touch_y);
            touch_start_timer = timer_read();
            pre_touch_x = touch_x;
            pre_touch_y = touch_y;
            // uprintf("g id: %d\n", gesture_id); 
            process_gesture(touch_data);
        }

    } else  {
        if (timer_elapsed(last_tap_time) > TOUCH_DEBOUNCE_TIME - 50) {
                touch_signal = false;
        }
    }
}

void load_virtual_keys(void) {
    for (int row = 0; row < MATRIX_ROWS / 2; row++) {
        for (int j = 0; j < MATRIX_COLS; j++) {
            import_keymaps[row][j] = keymap_key_to_keycode(1, (keypos_t){.row = row, .col = j});
        }
    }
    int i = 0;
    for (int row = 0; row < MATRIX_ROWS / 2; row++) {
        if (row == 0 || row == 1 || row == 2 || row == 3) {
            continue;
        }
        for (int col = 0; col < MATRIX_COLS; col++) {
            virtual_keycode[i] = import_keymaps[row][col];
            i++;
        }
    }
}

void save_omni_color_config(void) {
    dynamic_keymap_set_keycode(3,4,0, 0x7700 + hue_bg);
    dynamic_keymap_set_keycode(3,4,1, 0x7700 + sat_bg);
    dynamic_keymap_set_keycode(3,4,2, 0x7700 + val_bg);
    dynamic_keymap_set_keycode(3,5,0, 0x7700 + hue_main_color);
    dynamic_keymap_set_keycode(3,5,1, 0x7700 + sat_main_color);
    dynamic_keymap_set_keycode(3,5,2, 0x7700 + val_main_color);
    dynamic_keymap_set_keycode(3,5,3, 0x7700 + hue_sub_color);
    dynamic_keymap_set_keycode(3,5,4, 0x7700 + sat_sub_color);
    dynamic_keymap_set_keycode(3,5,5, 0x7700 + val_sub_color);
}

void load_omni_color_config(void) {
    hue_bg = dynamic_keymap_get_keycode(3,4,0) - 0x7700;
    sat_bg = dynamic_keymap_get_keycode(3,4,1) - 0x7700;
    val_bg = dynamic_keymap_get_keycode(3,4,2) - 0x7700;
    hue_main_color = dynamic_keymap_get_keycode(3,5,0) - 0x7700;
    sat_main_color = dynamic_keymap_get_keycode(3,5,1) - 0x7700;
    val_main_color = dynamic_keymap_get_keycode(3,5,2) - 0x7700;
    hue_sub_color = dynamic_keymap_get_keycode(3,5,3) - 0x7700;
    sat_sub_color = dynamic_keymap_get_keycode(3,5,4) - 0x7700;
    val_sub_color = dynamic_keymap_get_keycode(3,5,5) - 0x7700;
}

void save_omni_tb_config(void) {
    dynamic_keymap_set_keycode(3,7,0, 0x7700 + (int)round(speed_adjust1 * 10));
    dynamic_keymap_set_keycode(3,7,1, 0x7700 + slope_factor1);
    dynamic_keymap_set_keycode(3,7,2, 0x7700 + (int)round(speed_adjust2 * 10));
    dynamic_keymap_set_keycode(3,7,3, 0x7700 + slope_factor2);
}

void load_omni_tb_config(void) {
    speed_adjust1 =  (dynamic_keymap_get_keycode(3,7,0) - 0x7700) / 10.0f;
    slope_factor1 =  dynamic_keymap_get_keycode(3,7,1) - 0x7700;
    speed_adjust2 =  (dynamic_keymap_get_keycode(3,7,2) - 0x7700) / 10.0f;
    slope_factor2 =  dynamic_keymap_get_keycode(3,7,3) - 0x7700;
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


    if (dynamic_keymap_get_keycode(3,8,0) == 0x7701) {
        ud_sc_mode_flag = -1;
    } else if (dynamic_keymap_get_keycode(3,8,0) == 0x7700) {
        ud_sc_mode_flag = 1;
    }

    if (dynamic_keymap_get_keycode(3,8,1) == 0x7701) {
        lr_sc_mode_flag = -1;
    } else if (dynamic_keymap_get_keycode(3,8,1) == 0x7700) {
        lr_sc_mode_flag = 1;
    }

}

void show_trackball_tuning_mode(void) {
    draw_background_all_black();
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
    qp_curve(display, speed_adjust1, slope_factor1, hue_tbtune_r, sat_tbtune_r, val_tbtune_r, x_pos1, y_pos1);
    qp_curve(display, speed_adjust2, slope_factor2, hue_tbtune_l, sat_tbtune_l, val_tbtune_l, x_pos2, y_pos2);
    qp_drawimage(display, 120 - image_save->width/2, 210 - image_save->height/2, image_save);
    qp_flush(display);
}

void display_redraw(void) {
    switch (display_mode) {
        case DISPLAY_MODE_TOUCH_KEY:
            draw_background_all_black();
            draw_lcd_layer_category_images();
            break;
        case DISPLAY_MODE_TRACKBALL_TUNING:
            show_trackball_tuning_mode();
            break;
        case DISPLAY_MODE_SWIPE_GESTURE:
            draw_background_all();
            swipe_gesture_layer_view_update();
            swipe_gesture_base_view_update();
            swipe_gesture_main_view_update(current_lcd_layer);
            break;
        case DISPLAY_MODE_KEY_MATRIX:
            draw_key_matrix(display, roboto_mono16, st2_mono16, current_layer);
            break;
        default:
            break;
    }
}

void process_touch_trackball_tuning_mode(uint16_t touch_x, uint16_t touch_y) {
    const uint8_t radius = 22;
    const uint8_t radius_gap = 1;
    point_t plus1_center = {x_pos1 - (radius + radius_gap), y_pos4}; 
    point_t minus1_center = {x_pos1 - (radius + radius_gap), y_pos3};
    point_t plus2_center = {x_pos2 - (radius + radius_gap), y_pos4}; 
    point_t minus2_center = {x_pos2 - (radius + radius_gap), y_pos3};
    point_t plus3_center = {x_pos1 + (radius + radius_gap), y_pos3}; 
    point_t minus3_center = {x_pos1 + (radius + radius_gap), y_pos4};
    point_t plus4_center = {x_pos2 + (radius + radius_gap), y_pos3}; 
    point_t minus4_center = {x_pos2 + (radius + radius_gap), y_pos4};
    point_t save_center = {120, 215};
    if (is_touch_in_circle(touch_x, touch_y, plus1_center, radius)) {
        speed_adjust1 += 0.2;
        if (speed_adjust1 > 3.0f) speed_adjust1 = 3.0f;
    }
    else if (is_touch_in_circle(touch_x, touch_y, minus1_center, radius)) {
        speed_adjust1 -= 0.2;
        if (speed_adjust1 < 0.2f) speed_adjust1 =  0.2f;
    }
    else if (is_touch_in_circle(touch_x, touch_y, plus2_center, radius)) {
        speed_adjust2 += 0.2;
        if (speed_adjust2 > 3.0f) speed_adjust2 = 3.0f;
    }
    else if (is_touch_in_circle(touch_x, touch_y, minus2_center, radius)) {
        speed_adjust2 -= 0.2;
        if (speed_adjust2 < 0.2f) speed_adjust2 = 0.2f;
    }
    else if (is_touch_in_circle(touch_x, touch_y, plus3_center, radius)) {
        slope_factor1 += 5;
        if (slope_factor1 > 100) slope_factor1 = 100;
    }
    else if (is_touch_in_circle(touch_x, touch_y, minus3_center, radius)) {
        slope_factor1 -= 5;
        if (slope_factor1 < 10) slope_factor1 = 10;
    }
    else if (is_touch_in_circle(touch_x, touch_y, plus4_center, radius)) {
        slope_factor2 += 5;
        if (slope_factor2 > 100) slope_factor2 = 100;
    }
    else if (is_touch_in_circle(touch_x, touch_y, minus4_center, radius)) {
        slope_factor2 -= 5;
        if (slope_factor2 < 10) slope_factor2 = 10;
    }
    else if (is_touch_in_circle(touch_x, touch_y, save_center, radius)) {
        save_omni_tb_config();
        show_trackball_tuning_mode();
    }
    if (pre_speed_adjust1 != speed_adjust1 || pre_slope_factor1 != slope_factor1) {
        draw_background_black(135, 80, 205, 145);
        wait_ms(10);
        qp_curve(display, speed_adjust1, slope_factor1, hue_tbtune_r, sat_tbtune_r, val_tbtune_r, x_pos1, y_pos1);
    }else if (pre_speed_adjust2 != speed_adjust2 || pre_slope_factor2 != slope_factor2) {
        draw_background_black(35, 80, 105, 145);
        wait_ms(10);
        qp_curve(display, speed_adjust2, slope_factor2, hue_tbtune_l, sat_tbtune_l, val_tbtune_l, x_pos2, y_pos2);
    }
    pre_speed_adjust1 = speed_adjust1;
    pre_speed_adjust2 = speed_adjust2;
    pre_slope_factor1 = slope_factor1;
    pre_slope_factor2 = slope_factor2;
}

bool is_center_touch(uint16_t touch_x, uint16_t touch_y) {
    const uint16_t center_x = 120, center_y = 120, radius = 30;
    uint32_t distance_squared = (touch_x - center_x) * (touch_x - center_x) + (touch_y - center_y) * (touch_y - center_y);
    return distance_squared <= (radius * radius);
}

void update_lcd_view_data(void){
    if (memcmp(virtual_keycode, pre_virtual_keycode, KEYCODE_SIZE * sizeof(uint16_t)) != 0) {
        memcpy(pre_virtual_keycode, virtual_keycode, KEYCODE_SIZE * sizeof(uint16_t));
        draw_background_all_black();
        initialize_lcd_layer_app_images();
        draw_lcd_layer_category_images();
    }
}

int8_t is_usb_connected(void) {
    usbstate_t state = usbGetDriverStateI(&USB_DRIVER);
    // uprintf("USB State: %d\n", state); 
    return state;
}

bool power_on_lcd(void) {
    qp_power(display, true);
    wait_ms(10);
    // setPinOutput(BLK_PIN);
    writePinHigh(BLK_PIN);
    return  true;
}

bool power_off_lcd(void) {
    // setPinOutput(BLK_PIN);
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
    pmw33xx_init(0);         
    pmw33xx_init(1);         
    pmw33xx_set_cpi(0, 3000);
    pmw33xx_set_cpi(1, 3000);
    pointing_device_init_user();
}

void keyboard_post_init_kb(void) {
    if (!eeconfig_is_enabled()) {
        eeconfig_init();
    }
    load_omni_tb_config(); 
    load_omni_color_config();
    display = qp_gc9a01_make_spi_device(240, 240, CS_PIN, DC_PIN, RST_PIN, 4, 0); // パネル幅, パネル高さ,,,,SPIディバイザ,SPIモード
    qp_init(display, QP_ROTATION_0);
    noto11_font = qp_load_font_mem(font_noto11); 
    roboto_mono16 = qp_load_font_mem(font_roboto_mono16); 
    st2_mono16 = qp_load_font_mem(font_st2_mono16); 
    uint8_t init_status = cst816t_init(2);
    if (init_status == 0) {
        uprintf("Touch initialization failed.\n");
    } else {
        uprintf("Touch initialization successful.\n");
    }
    draw_background_all_black();
    initialize_images();
    qp_flush(display);
    if (image_logo != NULL) {
        int x = (240 - image_logo->width) / 2;
        int y = (240 - image_logo->height) / 2;
        my_anim = qp_animate(display, x, y, image_logo);
        lcd_fast_res_time = timer_read();
    }
}

void matrix_init_user(void) {
    setPinOutput(BLK_PIN);
    writePinHigh(BLK_PIN);
    i2c_init();
    setPinInputHigh(INT_PIN); 
    initialize_lcd_layer_app_images();
}


report_mouse_t  pointing_device_task_kb(report_mouse_t mouse_report) {

    current_layer = get_highest_layer(layer_state);
    if(display_mode == DISPLAY_MODE_SWIPE_GESTURE) {
        if (current_layer != pre_layer) {
            swipe_gesture_main_view_update(current_layer);
        }
    } else if (display_mode ==  DISPLAY_MODE_KEY_MATRIX) {
        if (!get_auto_mouse_enable()) {
            if(current_layer != pre_layer){
                draw_key_matrix(display, roboto_mono16, st2_mono16, current_layer);
            }            
        } else {
            if (current_layer != 3){
                if(pre_layer !=  3){
                    if(current_layer != pre_layer){
                        draw_key_matrix(display, roboto_mono16, st2_mono16, current_layer);
                    }
                }
            }
        }
    }
    pre_layer = current_layer;

    pmw33xx_report_t report0 = pmw33xx_read_burst(0); // Sensor #1
    pmw33xx_report_t report1 = pmw33xx_read_burst(1); // Sensor #2

    if (report0.motion.b.is_motion || report1.motion.b.is_motion) {
        tb_state = true;
    }else {
        tb_state = false;
    }

    if (tb_mode_r == TRACKBALL_CURSOR){
        process_cursor_report(&mouse_report, report0, speed_adjust1, slope_factor1, 1, 1, 2);
    } else if (tb_mode_r == TRACKBALL_TAP) {
        process_tap_report(&mouse_report, report0, speed_adjust2, slope_factor2, -1, -1, 10);
    }

    if (tb_mode_l == TRACKBALL_CURSOR){
        process_cursor_report(&mouse_report, report1, speed_adjust1, slope_factor1, -1, -1, 2);
    } else if (tb_mode_l == TRACKBALL_TAP) {
        process_tap_report(&mouse_report, report1, speed_adjust2, slope_factor2, 1, 1, 10);
    }

    if (ENABLE_TOUCH_UPDATE == 1) {
        if (touch_signal_view_update) {
            if (!is_backlight_off) {
                writePinLow(BLK_PIN);
                blink_start_time = timer_read();
                is_backlight_off = true;
            }
            if (is_backlight_off && timer_elapsed(blink_start_time) >= 50) {
                writePinHigh(BLK_PIN);
                is_backlight_off = false;
                touch_signal_view_update = false;
            }
        }
    }
    if (!is_first_frame) {
        if (is_second_frame) {
            draw_lcd_layer_category_images();
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
    if (matrix_changed || tb_state || touch_start_flag){ 
        sleeping_timer = timer_read();
        if (sleeping_state) {
            if (!lcd_is_on){
                lcd_is_on = power_on_lcd();
            }
            display_redraw();
            sleeping_state = false;
        }
    }
    if (!sleeping_state) {
        if (timer_elapsed(sleeping_timer) >= SLEEPING_KB_TIME) {
            lcd_is_on = power_off_lcd();
            sleeping_state = true;
        }
    } else if (SLEEP_VIEW == 0) {
        return;
    } else if(SLEEP_VIEW == 1) {
        if (timer_elapsed(draw_matrix_code_rain_timer) > 50) {
            draw_matrix_code_rain_timer = timer_read();
            if (!lcd_is_on){
                lcd_is_on = power_on_lcd();
            }
            if (!fast_draw_matrix_code_rain) {
                init_matrix_code_rain();
                fast_draw_matrix_code_rain = true;
            }
            update_matrix_code_rain();
            draw_matrix_code_rain(display, noto11_font);
        }
    } 
}

void housekeeping_task_user(void) {
    lcd_current_time = timer_read();
    if (lcd_current_time - lcd_fast_res_time > 3000) {
        if (is_first_frame) {
            is_first_frame = false;
            qp_stop_animation(my_anim);
            draw_background_all_black();
            initialize_lcd_layer_app_images();
        }
    }
    matrix_changed = get_last_matrix_state();
    sleeping_kb(matrix_changed);

    if (touch_start_flag) {
        if (timer_elapsed(touch_start_timer) >= TOUCH_DEBOUNCE_TIME) {
            touch_start_flag = false;
            gesture_id = GESTURE_NONE;
        }   
    }
}


bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    if (!record->event.pressed) {
        tb_mode_l = TRACKBALL_TAP;
        tb_mode_r = TRACKBALL_CURSOR;
        return true;
    }
    switch (keycode) {
        case KC_hue_bg_UP:
            hue_bg = (hue_bg + 16) % 256;
            break;
        case KC_hue_bg_DOWN:
            hue_bg = (hue_bg >= 16) ? (hue_bg - 16) : (hue_bg + 240); 
            break;
        case KC_sat_bg_UP:
            sat_bg = (sat_bg + 16 <= 254) ? sat_bg + 16 : 254;
            break;
        case KC_sat_bg_DOWN:
            sat_bg = (sat_bg > 16) ? sat_bg - 16 : 0;
            break;
        case KC_val_bg_UP:
            val_bg = (val_bg + 16 <= 254) ? val_bg + 16 : 254;
            break;
        case KC_val_bg_DOWN:
            val_bg = (val_bg > 16) ? val_bg - 16 : 0;
            break;
        case KC_hue_main_color_UP:
            hue_main_color = (hue_main_color + 16) % 256;
            break;
        case KC_hue_main_color_DOWN:
            hue_main_color = (hue_main_color >= 16) ? (hue_main_color - 16) : (hue_main_color + 240);
            break;
        case KC_sat_main_color_UP:
            sat_main_color = (sat_main_color + 16 <= 254) ? sat_main_color + 16 : 254;
            break;
        case KC_sat_main_color_DOWN:
            sat_main_color = (sat_main_color > 16) ? sat_main_color - 16 : 0;
            break;
        case KC_val_main_color_UP:
            val_main_color = (val_main_color + 16 <= 254) ? val_main_color + 16 : 254;
            break;
        case KC_val_main_color_DOWN:
            val_main_color = (val_main_color > 16) ? val_main_color - 16 : 0;
            break;
        case KC_hue_sub_color_UP:
            hue_sub_color = (hue_sub_color + 16) % 256;
            break;
        case KC_hue_sub_color_DOWN:
            hue_sub_color = (hue_sub_color >= 16) ? (hue_sub_color - 16) : (hue_sub_color + 240);
            break;
        case KC_sat_sub_color_UP:
            sat_sub_color = (sat_sub_color + 16 <= 254) ? sat_sub_color + 16 : 254;
            break;
        case KC_sat_sub_color_DOWN:
            sat_sub_color = (sat_sub_color > 16) ? sat_sub_color - 16 : 0;
            break;
        case KC_val_sub_color_UP:
            val_sub_color = (val_sub_color + 16 <= 254) ? val_sub_color + 16 : 254;
            break;
        case KC_val_sub_color_DOWN:
            val_sub_color = (val_sub_color > 16) ? val_sub_color - 16 : 0;
            break;
        case AUTO_MOUSE_TOGGLE:
            if (dynamic_keymap_get_keycode(3,9,0) == 0x7700) {
                dynamic_keymap_set_keycode(3,9,0, 0x7701);
                set_auto_mouse_enable(true);
            } else if (dynamic_keymap_get_keycode(3,9,0) == 0x7701) {
                dynamic_keymap_set_keycode(3,9,0, 0x7700);
                set_auto_mouse_enable(false);
            }
            break;
        case TB_R_MODE_TOGGLE:
            if (record->event.pressed) {
                tb_mode_r = TRACKBALL_TAP;
            } else {
                tb_mode_r = TRACKBALL_CURSOR;
            }
            return false;
        case TB_L_MODE_TOGGLE:
            tb_mode_l = TRACKBALL_CURSOR;
            return false;

        case TB_UD_SC_MODE_TOGGLE:
            if (dynamic_keymap_get_keycode(3,8,0) == 0x7700) {
                dynamic_keymap_set_keycode(3,8,0, 0x7701);
                ud_sc_mode_flag = -1;
            } else if (dynamic_keymap_get_keycode(3,8,0) == 0x7701) {
                dynamic_keymap_set_keycode(3,8,0, 0x7700);
                ud_sc_mode_flag = 1;
            }
            break;

        case TB_LR_SC_MODE_TOGGLE:
            if (dynamic_keymap_get_keycode(3,8,1) == 0x7700) {
                dynamic_keymap_set_keycode(3,8,1, 0x7701);
                lr_sc_mode_flag = -1;
            } else if (dynamic_keymap_get_keycode(3,8,1) == 0x7701) {
                dynamic_keymap_set_keycode(3,8,1, 0x7700);
                lr_sc_mode_flag = 1;
            }
            break;

        case KC_DP_TOUCH_KEY:
            display_mode =  DISPLAY_MODE_TOUCH_KEY;
            draw_background_all_black();
            draw_lcd_layer_category_images();
            break;
        case KC_DP_TB_TUNE: 
            display_mode =  DISPLAY_MODE_TRACKBALL_TUNING;
            show_trackball_tuning_mode();
            break;
        case KC_DP_SWIPE_GESTURE:
            display_mode =  DISPLAY_MODE_SWIPE_GESTURE;
            draw_background_all();
            swipe_gesture_layer_view_update();
            swipe_gesture_base_view_update();
            swipe_gesture_main_view_update(current_lcd_layer);
            break;
        case KC_DP_KEY_MAT:
            display_mode =  DISPLAY_MODE_KEY_MATRIX;
            draw_key_matrix(display, roboto_mono16, st2_mono16, current_layer);
            break;
        default:
            return true;
    }
    if(display_mode == DISPLAY_MODE_SWIPE_GESTURE) {
        save_omni_color_config();
        draw_background_all();
        swipe_gesture_base_view_update();
        swipe_gesture_main_view_update(current_lcd_layer);
        swipe_gesture_layer_view_update();
    }
    return false;
}

void suspend_power_down_user(void){
    lcd_is_on = power_off_lcd();
    sleeping_state = true;
};



