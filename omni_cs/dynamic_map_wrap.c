#include "dynamic_keymap.h"
#include "print.h"

/* GUI→キーボードのキー書き換えごとに呼びたい処理 */
static void my_keymap_changed(uint8_t layer,
                              uint8_t row,
                              uint8_t col,
                              uint16_t keycode)
{
    uprintf("changed: L%d R%d C%d KC%04X\n", layer, row, col, keycode);
}

/* リンカが自動で作る「元の関数」を extern 宣言 */
extern void __real_dynamic_keymap_set_keycode(uint8_t layer,
                                              uint8_t row,
                                              uint8_t col,
                                              uint16_t keycode);

/* wrap 本体 */
void __wrap_dynamic_keymap_set_keycode(uint8_t layer,
                                       uint8_t row,
                                       uint8_t col,
                                       uint16_t keycode)
{
    /* ① 元処理：EEPROM 書き込み等 */
    __real_dynamic_keymap_set_keycode(layer, row, col, keycode);
    /* ② 追加処理：GUI からの変更をキャッチ */
    my_keymap_changed(layer, row, col, keycode);
}
