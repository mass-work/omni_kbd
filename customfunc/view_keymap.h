#pragma once

#include "quantum.h"

#define DISPLAY_WIDTH 200
#define DISPLAY_HEIGHT 80
#define MARGIN_X 2
#define MARGIN_Y 2

#define VISIBLE_WIDTH  (DISPLAY_WIDTH - 2 * MARGIN_X)  
#define VISIBLE_HEIGHT (DISPLAY_HEIGHT - 2 * MARGIN_Y)

#define LOGICAL_COLS 14
#define KEY_ROWS 4
#define KEY_WIDTH  (VISIBLE_WIDTH / LOGICAL_COLS)      
#define KEY_HEIGHT (VISIBLE_HEIGHT / KEY_ROWS)        

typedef enum {
    KEYBOARD_LAYOUT_US,
    KEYBOARD_LAYOUT_JIS
} keyboard_layout_t;

extern keyboard_layout_t current_layout;

const char* get_display_string_from_keycode(uint16_t keycode);

void draw_key_matrix(painter_device_t display, painter_font_handle_t font1, painter_font_handle_t font2, uint8_t current_layer);

// 独自グリフマクロの定義（ASCIIにマッピングした場合）
#define F1   ' '
#define F2   '!'
#define F3   '"'
#define F4   '#'
#define F5   '$'
#define F6   '%'
#define F7   '&'
#define F8   '\''
#define F9   '('
#define F10   ')'
#define F11   '*'
#define F12   '+'
#define F13   ','
#define F14   '-'
#define F15   '.'

#define CL   '/'
#define CR   '0'
#define CS   '1'
#define SH   '2'
#define CT   '3'
#define AL   '4'
#define GU   '5'
#define OP   '6'
#define CM   '7'
#define FN   '8'
#define SP   '9'
#define EN   ':'
#define BS   ';'
#define TA   '<'

#define L0   '='
#define L1   '>'
#define L2   '?'
#define L3   '@'
#define L4   'A'
#define L5   'B'
#define L6   'C'
#define L7   'D'
#define L8   'E'
#define L9   'F'

#define ES   'G'
#define DE   'H'