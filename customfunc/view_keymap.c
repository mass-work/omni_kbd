// Copyright 2025 mass
// SPDX-License-Identifier: GPL-2.0-or-later

#include "view_keymap.h"
#include "qp.h"
#include "dynamic_keymap.h"
#include <print.h>
#include "../omni_cs/omni_cs.h"


keyboard_layout_t current_layout = KEYBOARD_LAYOUT_JIS;


// USは未対応
const char* get_display_string_us(uint16_t keycode) {
    // if ((keycode & 0xFF) == KC_NO) return " ";
    // uint16_t base = keycode;
    // if (base >= KC_A && base <= KC_Z) {
    //     static char letter[2];
    //     letter[0] = 'a' + (base - KC_A);
    //     letter[1] = '\0';
    //     return letter;
    // }
    // if (base >= KC_1 && base <= KC_9) {
    //     static const char* nums[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
    //     return nums[base - KC_1];
    // } else if (base == KC_0) {
    //     return "0";
    // }
    // if (base == KC_SPACE) return " ";
    // if (base == KC_LSFT) return "S";
    // if (base == KC_LCTL) return "C";
    // if (base == KC_LGUI)   return "G";
    // if (base == KC_LALT)   return "A";
    // if (base == KC_RSFT) return "S";
    // if (base == KC_RCTL) return "C";
    // if (base == KC_RGUI)   return "G";   
    // if (base == KC_RALT)   return "A";
    // if (base == KC_ENTER) return "E";
    // if (base == KC_TAB)   return "T";
    // if (base == KC_BSPC)  return "B";
    // if (base == KC_ESC)   return "E";
    // if (base == KC_MINUS) return "-";
    // if (base == KC_EQUAL) return "=";
    // if (base == KC_LBRC)  return "[";
    // if (base == KC_RBRC)  return "]";
    // if (base == KC_BSLS)  return "\\";
    // if (base == KC_SCLN)  return ";";
    // if (base == KC_QUOT)  return "'";
    // if (base == KC_COMM)  return ",";
    // if (base == KC_DOT)   return ".";
    // if (base == KC_SLSH)  return "/";
    // if (base == KC_EXLM)  return "!";
    // if (base == KC_AT)    return "@";
    // if (base == KC_HASH)  return "#";
    // if (base == KC_DLR)   return "$";
    // if (base == KC_PERC)  return "%";
    // if (base == KC_CIRC)  return "^";
    // if (base == KC_AMPR)  return "&";
    // if (base == KC_ASTR)  return "*";
    // if (base == KC_LPRN)  return "(";
    // if (base == KC_RPRN)  return ")";
    return "-";
}


const char* get_display_string_jis(uint16_t keycode) {
    if ((keycode) == KC_NO) return " ";
    uint16_t base = keycode;
    if (base >= KC_A && base <= KC_Z) {
        static char letter[2];
        letter[0] = 'a' + (base - KC_A);
        letter[1] = '\0';
        return letter;
    }
    if (base >= KC_1 && base <= KC_9) {
        static const char* nums[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
        return nums[base - KC_1];
    } else if (base == KC_0) {
        return "0";
    }
    if (base >= KC_F1 && base <= KC_F12) {
        static const char* nums[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" ,"A", "B", "C"};
        return nums[base - KC_F1];
    } else  if (base >= KC_F13 && base <= KC_F16) {
        static const char* nums[] = { "D", "E", "F", "G"};
        return nums[base - KC_F13];
    } 

    if (base == KC_BTN1) return "CL";
    if (base == KC_BTN2) return "CR";
    if (base == KC_BTN3) return "CS";
    if (base == KC_SPACE) return "SP";
    if (base == KC_LSFT) return "SH";
    if (base == KC_LCTL) return "CT";
    if (base == KC_LGUI)   return "G";
    if (base == KC_LALT)   return "A";
    if (base == KC_RSFT) return "S  ";
    if (base == KC_RCTL) return "C";
    if (base == KC_RGUI)   return "G";
    if (base == KC_RALT)   return "A";
    if (base == KC_ENTER) return "E";
    if (base == KC_TAB)   return "T";
    if (base == KC_BSPC)  return "B";
    if (base == KC_DEL)  return "DE";
    if (base == KC_ESC)   return "ES";
    if (base == KC_LBRC)  return "@";
    if (base == 48)  return "[";
    if (base == 50)  return "]";
    if (base == KC_SCLN)  return ";";
    if (base == KC_QUOT)  return ":";
    if (base == KC_COMM)  return ",";
    if (base == KC_DOT)   return ".";
    if (base == KC_SLSH)  return "/";
    if (base == KC_INTERNATIONAL_3) return "¥";
    if (base == KC_EXLM)  return "!";
    if (base == KC_INT1)  return "\\";
    if (base == KC_HASH)  return "#";
    if (base == KC_DLR)   return "$";
    if (base == KC_PERC)  return "%";
    if (base == 547)  return "&";
    if (base == KC_QUES)  return "?";
    if (base == 549)  return "(";
    if (base == 550)  return ")";
    if (base == 558) return "~";
    if (base == 649)  return "|";
    if (base == 548) return "'";
    if (base == 559) return "`";
    if (base == 543) return "\"";
    if (base == KC_LABK)  return "<";
    if (base == KC_RABK)  return ">";
    if (base == KC_UNDS)  return "_";
    if (base == 87)  return "+";
    if (base == 86) return "-";
    if (base == 85)  return "*";
    if (base == 84)  return "/";
    if (base == KC_EQUAL) return "^";
    if (base == 560)  return "{";
    if (base == 562)  return "}";
    if (base == KC_UP)  return "^";
    if (base == KC_DOWN)  return "v";
    if (base == KC_LEFT)  return "<";
    if (base == KC_RIGHT)  return ">";
    return "-";
}

const char* get_display_string_from_keycode(uint16_t keycode) {

    if ((keycode & 0xFF) == KC_NO) {
        return " ";  
    }

    switch (current_layout) {
        case KEYBOARD_LAYOUT_JIS:
            return get_display_string_jis(keycode);
        case KEYBOARD_LAYOUT_US:
        default:
            return get_display_string_us(keycode);
    }
}

typedef struct {
    const char* label;
    bool is_special;
} display_label_t;

display_label_t get_display_label(uint16_t keycode) {
    display_label_t dl = { .label = " ", .is_special = false };

    switch (current_layout) {
        case KEYBOARD_LAYOUT_JIS:
            dl.label = get_display_string_jis(keycode);
            break;
        case KEYBOARD_LAYOUT_US:
        default:
            dl.label = get_display_string_us(keycode);
            break;
    }

    #define MAKE_LABEL(name, ch) static char name[2] = {ch, 0}

    MAKE_LABEL(lbl_cl, CL);
    MAKE_LABEL(lbl_cr, CR);
    MAKE_LABEL(lbl_cs, CS);
    MAKE_LABEL(lbl_sh, SH);
    MAKE_LABEL(lbl_ct, CT);
    MAKE_LABEL(lbl_al, AL);
    MAKE_LABEL(lbl_gu, GU);
    MAKE_LABEL(lbl_sp, SP);
    MAKE_LABEL(lbl_en, EN);
    MAKE_LABEL(lbl_bs, BS);
    MAKE_LABEL(lbl_ta, TA);
    MAKE_LABEL(lbl_es, ES);
    MAKE_LABEL(lbl_de, DE);
    MAKE_LABEL(lbl_l1, L1);
    MAKE_LABEL(lbl_l2, L2);
    MAKE_LABEL(lbl_l3, L3);
    MAKE_LABEL(lbl_l4, L4);
    MAKE_LABEL(lbl_l5, L5);
    MAKE_LABEL(lbl_l6, L6);
    MAKE_LABEL(lbl_l7, L7);
    MAKE_LABEL(lbl_l8, L8);
    MAKE_LABEL(lbl_l9, L9);

    switch (keycode) {
        case KC_BTN1:   dl.label = lbl_cl; dl.is_special = true; break;
        case KC_BTN2:   dl.label = lbl_cr; dl.is_special = true; break;
        case KC_BTN3:   dl.label = lbl_cs; dl.is_special = true; break;
        case KC_LSFT:
        case KC_RSFT:   dl.label = lbl_sh; dl.is_special = true; break;
        case KC_LCTL:
        case KC_RCTL:   dl.label = lbl_ct; dl.is_special = true; break;
        case KC_LALT:
        case KC_RALT:   dl.label = lbl_al; dl.is_special = true; break;
        case KC_LGUI:
        case KC_RGUI:   dl.label = lbl_gu; dl.is_special = true; break;
        case KC_SPC:    dl.label = lbl_sp; dl.is_special = true; break;
        case KC_ENTER:  dl.label = lbl_en; dl.is_special = true; break;
        case KC_BSPC:   dl.label = lbl_bs; dl.is_special = true; break;
        case KC_TAB:    dl.label = lbl_ta; dl.is_special = true; break;
        case KC_ESC:    dl.label = lbl_es; dl.is_special = true; break;
        case KC_DEL:    dl.label = lbl_de; dl.is_special = true; break;

        case KC_F1 ... KC_F12: {
            static char f_label[2];
            f_label[0] = F1 + (keycode - KC_F1);
            f_label[1] = 0;
            dl.label = f_label;
            dl.is_special = true;
            break;
        }
        case KC_F13 ... KC_F15: {
            static char f_label[2];
            f_label[0] = F13 + (keycode - KC_F13);
            f_label[1] = 0;
            dl.label = f_label;
            dl.is_special = true;
            break;
        }

        case MO(1): dl.label = lbl_l1; dl.is_special = true; break;
        case MO(2): dl.label = lbl_l2; dl.is_special = true; break;
        case MO(3): dl.label = lbl_l3; dl.is_special = true; break;
        case MO(4): dl.label = lbl_l4; dl.is_special = true; break;
        case MO(5): dl.label = lbl_l5; dl.is_special = true; break;
        case MO(6): dl.label = lbl_l6; dl.is_special = true; break;
        case MO(7): dl.label = lbl_l7; dl.is_special = true; break;
        case MO(8): dl.label = lbl_l8; dl.is_special = true; break;
        case MO(9): dl.label = lbl_l9; dl.is_special = true; break;
    }

    return dl;
}


void draw_key_matrix(painter_device_t display, painter_font_handle_t font1, painter_font_handle_t font2, uint8_t current_layer) {
    qp_rect(display, 0, 0, 240, 240, 0, 0, 0, true);
    const char *layer_text = get_layer_name(current_layer);
    uint16_t layer_text_width = qp_textwidth(font1, layer_text);
    qp_drawtext(display, 120 - layer_text_width / 2, 40 - font1->line_height / 2, font1, layer_text);
    // uprintf("current_layer: %d\n", current_layer);  

    for (uint8_t row = 0; row < 3; row++) {
        for (uint8_t col = 0; col < 12; col++) {
            uint8_t matrix_row = (col < 6) ? row : (row + 16);
            uint8_t matrix_col = col % 6;

            uint8_t adjusted_col = (col < 6) ? col : col + 3;
            int x = MARGIN_X + adjusted_col * KEY_WIDTH + 5;
            int y = MARGIN_Y + row * KEY_HEIGHT + 70;

            uint16_t keycode = dynamic_keymap_get_keycode(current_layer, matrix_row, matrix_col);
            // uprintf("r: %d, c: %d, code: %d\n", matrix_row, matrix_col, keycode);     
            display_label_t dl = get_display_label(keycode);
            qp_drawtext(display, x + KEY_WIDTH / 2, y + KEY_HEIGHT / 2, dl.is_special ? font2 : font1, dl.label);
        }
    }

    {
        uint8_t row = 3;
        int y = MARGIN_Y + row * KEY_HEIGHT + 70;


        for (uint8_t col = 0; col < 6; col++) {
            int x = MARGIN_X + col * KEY_WIDTH + 5;
            uint16_t keycode = dynamic_keymap_get_keycode(current_layer, 3, col);
            // uprintf("r: %d, c: %d, code: %d\n", 3, col, keycode);     
            display_label_t dl = get_display_label(keycode);
            qp_drawtext(display, x + KEY_WIDTH / 2, y + KEY_HEIGHT / 2, dl.is_special ? font2 : font1, dl.label);
        }

        // R31
        int x = MARGIN_X + 6 * KEY_WIDTH + 5;
        uint16_t keycode = dynamic_keymap_get_keycode(current_layer, 19, 1);
        // uprintf("r: %d, c: %d, code: %d\n", 19, 1, keycode); 
        display_label_t dl = get_display_label(keycode);
        qp_drawtext(display, x + KEY_WIDTH / 2, y + KEY_HEIGHT / 2, dl.is_special ? font2 : font1, dl.label);

        x = MARGIN_X + 8 * KEY_WIDTH + 5;
        keycode = dynamic_keymap_get_keycode(current_layer, 19, 2);
        // uprintf("r: %d, c: %d, code: %d\n", 19, 2, keycode); 
        dl = get_display_label(keycode);
        qp_drawtext(display, x + KEY_WIDTH / 2, y + KEY_HEIGHT / 2, dl.is_special ? font2 : font1, dl.label);

        x = MARGIN_X + 9 * KEY_WIDTH + 5;
        keycode = dynamic_keymap_get_keycode(current_layer, 19, 3);
        // uprintf("r: %d, c: %d, code: %d\n", 19, 3, keycode); 
        dl = get_display_label(keycode);
        qp_drawtext(display, x + KEY_WIDTH / 2, y + KEY_HEIGHT / 2, dl.is_special ? font2 : font1, dl.label);

        x = MARGIN_X + 14 * KEY_WIDTH + 5;
        keycode = dynamic_keymap_get_keycode(current_layer, 19, 5);
        // uprintf("r: %d, c: %d, code: %d\n", 19, 5, keycode); 
        dl = get_display_label(keycode);
        qp_drawtext(display, x + KEY_WIDTH / 2, y + KEY_HEIGHT / 2, dl.is_special ? font2 : font1, dl.label);

        x = MARGIN_X + 1.5 * KEY_WIDTH + 5;
        y = MARGIN_Y + 4 * KEY_HEIGHT + 70;
        keycode = dynamic_keymap_get_keycode(current_layer, 19, 0);
        // uprintf("r: %d, c: %d, code: %d\n", 19, 0, keycode); 
        dl = get_display_label(keycode);
        qp_drawtext(display, x + KEY_WIDTH / 2, y + KEY_HEIGHT / 2, dl.is_special ? font2 : font1, dl.label);
    }

    qp_flush(display);
}
