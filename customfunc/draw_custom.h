#pragma once

#include "quantum.h"



#define CURVE_POINTS 100
#define MAX_HID_VALUE 127
#define MIN_HID_VALUE -127

// bool qp_curve(painter_device_t device, float delta_x, float delta_y, float speed_adjust, uint16_t cpi, uint8_t hue, uint8_t sat, uint8_t val);
bool qp_curve(painter_device_t device, float speed_adjust,int slope_factor, uint16_t cpi, uint8_t hue, uint8_t sat, uint8_t val, uint8_t x_pos, uint8_t y_pos);
