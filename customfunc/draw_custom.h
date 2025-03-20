#pragma once

#include "quantum.h"

#define CURVE_POINTS 100
#define MAX_HID_VALUE 127
#define MIN_HID_VALUE -127

bool qp_curve(painter_device_t device, float speed_adjust,int slope_factor, uint8_t hue, uint8_t sat, uint8_t val, uint8_t x_pos, uint8_t y_pos);
bool qp_fill_arc(painter_device_t device, uint16_t centerx, uint16_t centery, uint16_t outer_radius, uint16_t inner_radius, uint16_t start_angle, uint16_t end_angle, uint8_t hue, uint8_t sat, uint8_t val);
void qp_donut(painter_device_t device, uint16_t x, uint16_t y, uint16_t radius, uint16_t thickness, uint8_t hue, uint8_t sat, uint8_t val1, uint8_t val2);
