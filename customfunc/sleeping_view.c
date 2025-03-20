// Copyright 2025 mass
// SPDX-License-Identifier: GPL-2.0-or-later

#include <stdint.h>   
#include <stdlib.h>   
#include <math.h>
#include "qp.h"       
#include "../font/noto11.qff.h"
#include "../omni_CS/omni_cs.h"
#include "sleeping_view.h"

uint8_t hue_bg;
uint8_t sat_bg;
uint8_t val_bg;
uint8_t hue_main_color;
uint8_t sat_main_color;
uint8_t val_main_color;
uint8_t hue_sub_color;
uint8_t sat_sub_color;
uint8_t val_sub_color;
static CodeDrop codeDrops[RAIN_MAX_COLUMNS];
static const char characters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
static uint8_t char_count = sizeof(characters) - 1;

void init_matrix_code_rain(void) {
    for (int i = 0; i < RAIN_MAX_COLUMNS; i++) {
        codeDrops[i].x = rand() % SCREEN_WIDTH;  // ランダムなX座標を設定
        codeDrops[i].y = rand() % (SCREEN_HEIGHT / 2);  // 上半分に配置
        codeDrops[i].character = characters[rand() % char_count];
        codeDrops[i].fade = FADE_STEPS;  // 初期のフェード値を設定
    }
}

void draw_matrix_code_rain(painter_device_t display, painter_font_handle_t font) {
    qp_rect(display, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,  hue_bg, sat_bg, val_bg, true);
    for (int i = 0; i < RAIN_MAX_COLUMNS; i++) {  
        if (codeDrops[i].fade > 0) {
            char str[2] = {codeDrops[i].character, '\0'};
            uint8_t brightness = 255 * codeDrops[i].fade / FADE_STEPS;
            if (brightness < 30) brightness = 30; 
            qp_drawtext_recolor(display, codeDrops[i].x, codeDrops[i].y, font, str, 
                                hue_main_color, sat_main_color, brightness, hue_bg, sat_bg, val_bg);
        }
    }
    qp_flush(display); 
}

void update_matrix_code_rain(void) {
    for (int i = 0; i < RAIN_MAX_COLUMNS; i++) {
        codeDrops[i].y += FALL_SPEED;
        if (codeDrops[i].y > SCREEN_HEIGHT) {
            codeDrops[i].y = rand() % (SCREEN_HEIGHT / 2);
            codeDrops[i].x = rand() % SCREEN_WIDTH;
            codeDrops[i].character = characters[rand() % char_count];
            codeDrops[i].fade = FADE_STEPS;
        } else {
            codeDrops[i].fade--;
        }
    }
}


