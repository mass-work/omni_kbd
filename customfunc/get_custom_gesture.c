// Copyright 2025 mass
// SPDX-License-Identifier: GPL-2.0-or-later
#include "get_custom_gesture.h"
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "timer.h"

static bool touch_active = false;
static uint16_t last_x = 0;
static uint16_t last_y = 0;
static uint32_t last_update_time = 0;
static GestureType last_sent_gesture = CUSTOM_GESTURE_NONE;
static uint32_t last_gesture_sent_time = 0;
static int detection_count = 0;  // 連続検出回数
static bool first_update_after_touch_start = false;

GestureType get_custom_swipe_gesture(uint16_t x, uint16_t y) {
    uint32_t now = timer_read();
    bool is_touching = (x != 0 || y != 0);
    if (is_touching) {
        if (!touch_active) {
            touch_active = true;
            last_x = x;
            last_y = y;
            last_update_time = now;
            last_sent_gesture = CUSTOM_GESTURE_NONE;
            detection_count = 0;
            first_update_after_touch_start = true;
            return CUSTOM_GESTURE_NONE;
        } else {
            if (timer_elapsed(last_update_time) >= GESTURE_UPDATE_FREQ) {
                int delta_x = (int)x - (int)last_x;
                int delta_y = (int)y - (int)last_y;
                GestureType current = CUSTOM_GESTURE_NONE;
                if (abs(delta_x) < MIN_MOVEMENT_THRESHOLD && abs(delta_y) < MIN_MOVEMENT_THRESHOLD) {
                    current = CUSTOM_GESTURE_NONE;
                } else {
                    if (abs(delta_x) >= abs(delta_y)) {
                        current = (delta_x > 0) ? CUSTOM_GESTURE_RIGHT : CUSTOM_GESTURE_LEFT;
                    } else {
                        current = (delta_y > 0) ? CUSTOM_GESTURE_DOWN : CUSTOM_GESTURE_UP;
                    }
                }
                last_update_time = now;
                last_x = x;
                last_y = y;

                if (first_update_after_touch_start) {
                    first_update_after_touch_start = false;
                    return CUSTOM_GESTURE_NONE;
                }

                if (current != CUSTOM_GESTURE_NONE) {
                    if (current != last_sent_gesture) {
                        last_sent_gesture = current;
                        detection_count = 1;
                        last_gesture_sent_time = now;
                    } else {
                        uint32_t required_interval = (detection_count == 1) ? INITIAL_GESTURE_INTERVAL : GESTURE_UPDATE_FREQ;
                        if (timer_elapsed(last_gesture_sent_time) >= required_interval) {
                            detection_count++;
                            last_gesture_sent_time = now;
                        } else {
                            current = CUSTOM_GESTURE_NONE;
                        }
                    }
                } else {
                    last_sent_gesture = CUSTOM_GESTURE_NONE;
                    detection_count = 0;
                }
                return current;
            }
        }
        return CUSTOM_GESTURE_NONE;
    } else {
        if (touch_active) {
            touch_active = false;
            last_sent_gesture = CUSTOM_GESTURE_NONE;
            detection_count = 0;
            first_update_after_touch_start = false;
        }
        return CUSTOM_GESTURE_NONE;
    }
}
