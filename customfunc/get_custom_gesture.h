#pragma once

#include <stdbool.h>

#define GESTURE_UPDATE_FREQ     50
#define INITIAL_GESTURE_INTERVAL 150
#define MIN_MOVEMENT_THRESHOLD   20
#define MAX_DELTA_THRESHOLD      500


// ジェスチャーの種類
typedef enum {
    CUSTOM_GESTURE_NONE,
    CUSTOM_GESTURE_CW,
    CUSTOM_GESTURE_CCW,
    CUSTOM_GESTURE_RIGHT,
    CUSTOM_GESTURE_LEFT,
    CUSTOM_GESTURE_UP,
    CUSTOM_GESTURE_DOWN
} GestureType;

GestureType get_custom_swipe_gesture(uint16_t x, uint16_t y) ;
