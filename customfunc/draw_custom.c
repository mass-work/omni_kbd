#include "draw_custom.h"
#include "config.h"

#include <math.h>
#include <stdint.h>
#include "qp_internal.h"
#include "qp_comms.h"
#include "qp_draw.h"
#include "qgf.h"


static inline int16_t constrain_hid(int16_t value) {
    if (value > MAX_HID_VALUE) return MAX_HID_VALUE;
    if (value < MIN_HID_VALUE) return MIN_HID_VALUE;
    return value;
}

bool qp_curve(painter_device_t device, float speed_adjust, int slope_factor, uint16_t cpi, uint8_t hue, uint8_t sat, uint8_t val, uint8_t x_pos, uint8_t y_pos) {

    // デバイスの検証
    painter_driver_t *driver = (painter_driver_t *)device;
    if (!driver || !driver->validate_ok) {
        // qp_dprintf("qp_curve: fail (validation_ok == false)\n");
        return false;
    }

    // 通信開始
    if (!qp_comms_start(device)) {
        // qp_dprintf("qp_curve: fail (comms_start == false)\n");
        return false;
    }

    // 色データを設定
    qp_internal_fill_pixdata(device, 1, hue, sat, val);



    // 描画用の中心座標を定義
    int center_x = x_pos; // ディスプレイの中心X
    int center_y = y_pos; // ディスプレイの中心Y

    // デルタ値を固定配列として定義 (-100 ～ 100 の範囲)
    int draw_size = 70;
    int draw_scale_y = 110;
    int hurf_value = draw_size / 2;
    int delta_values[draw_size + 1];
    for (int i = 0; i < draw_size + 1; i++) {
        delta_values[i] = i; // draw_sizeの配列を作成
    }

    bool ret = true;
    for (int i = 0; i < draw_size; i++) {
        // 配列から delta_x, delta_y を取得
        float delta_x0 = delta_values[i];
        float delta_x1 = delta_values[i+1];
        float delta_y0 = delta_values[i]; // 次の点
        float delta_y1 = delta_values[i+1]; // 次の点

        // 補正値を計算
        float scaled_y0 = pow(delta_y0, speed_adjust) / pow(draw_size, speed_adjust) * draw_size / draw_scale_y * slope_factor;
        float scaled_y1 = pow(delta_y1, speed_adjust) / pow(draw_size, speed_adjust) * draw_size / draw_scale_y * slope_factor;

        // 描画座標を計算
        int draw_x0 = (center_x - hurf_value) + (int)delta_x0;
        int draw_x1 = (center_x - hurf_value) + (int)delta_x1;
        int draw_y0 = center_y + hurf_value - (int)scaled_y0; // 上下反転
        int draw_y1 = center_y + hurf_value - (int)scaled_y1; // 上下反転
        // qp_line(device, draw_x0, draw_y0, draw_x1, draw_y1, hue, sat, val);
        // qp_line(device, draw_x0, draw_y0, draw_x1, draw_y1, hue, sat, val);

        // 範囲チェック
        if (draw_x0 >= 0 && draw_x0 < 240 && draw_y0 >= 0 && draw_y0 < 240 &&
            draw_x1 >= 0 && draw_x1 < 240 && draw_y1 >= 0 && draw_y1 < 240) {
            // qp_line を使用して連続的な曲線を描画js
            if (!qp_internal_setpixel_impl(device, draw_x0, draw_y0) ||
                !qp_internal_setpixel_impl(device, draw_x1, draw_y1)) {
                ret = false;
                break;
            }
        }
        // qp_flush(device);
    }

    // // 通信終了
    qp_comms_stop(device);
    qp_dprintf("qp_curve: %s\n", ret ? "ok" : "fail");

    return ret;
}
