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

bool qp_curve(painter_device_t device, float speed_adjust, int slope_factor, uint8_t hue, uint8_t sat, uint8_t val, uint8_t x_pos, uint8_t y_pos) {

    // デバイスの検証
    painter_driver_t *driver = (painter_driver_t *)device;
    if (!driver || !driver->validate_ok) {
        return false;
    }

    // 通信開始
    if (!qp_comms_start(device)) {
        return false;
    }

    qp_internal_fill_pixdata(device, 1, hue, sat, val);

    int center_x = x_pos;
    int center_y = y_pos;

    int draw_size = 70;
    int draw_scale_y = 110;
    int hurf_value = draw_size / 2;
    int delta_values[draw_size + 1];
    for (int i = 0; i < draw_size + 1; i++) {
        delta_values[i] = i;
    }

    bool ret = true;
    for (int i = 0; i < draw_size; i++) {
        float delta_x0 = delta_values[i];
        float delta_x1 = delta_values[i+1];
        float delta_y0 = delta_values[i];
        float delta_y1 = delta_values[i+1];

        float scaled_y0 = pow(delta_y0, speed_adjust) / pow(draw_size, speed_adjust) * draw_size / draw_scale_y * slope_factor;
        float scaled_y1 = pow(delta_y1, speed_adjust) / pow(draw_size, speed_adjust) * draw_size / draw_scale_y * slope_factor;

        int draw_x0 = (center_x - hurf_value) + (int)delta_x0;
        int draw_x1 = (center_x - hurf_value) + (int)delta_x1;
        int draw_y0 = center_y + hurf_value - (int)scaled_y0;
        int draw_y1 = center_y + hurf_value - (int)scaled_y1;

        if (draw_x0 >= 0 && draw_x0 < 240 && draw_y0 >= 0 && draw_y0 < 240 &&
            draw_x1 >= 0 && draw_x1 < 240 && draw_y1 >= 0 && draw_y1 < 240) {
            if (!qp_internal_setpixel_impl(device, draw_x0, draw_y0) ||
                !qp_internal_setpixel_impl(device, draw_x1, draw_y1)) {
                ret = false;
                break;
            }
        }
    }

    qp_comms_stop(device);
    qp_dprintf("qp_curve: %s\n", ret ? "ok" : "fail");

    return ret;
}
