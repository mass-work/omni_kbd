// Copyright 2025 mass
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <stdlib.h>		//itoa()
#include <stdio.h>

// #define address   0x15  //slave address
#define CST816S_ADDR (0x15 << 1)  // 7ビットアドレスを左シフト

#define GESTUREID 0x01 //gesture code
#define GESTURE_NONE 0x00 		//no gesture
#define CST816S_SLIDE_UP 0x01   // slide up
#define CST816S_SLIDE_DOWN 0x02 // slide down
#define CST816S_SLIDE_LEFT 0x03 // slide left
#define CST816S_SLIDE_RIGHT 0x04 // slide right
#define CST816S_CLICK 0x05		//click
#define CST816S_DOUBLE_CLICK 0x0B//double click
#define CST816S_LONG_PRESS 0x0C	//Press

#define FingerNum 0X02
#define XposH 0x03 //X coordinate high 4 digits
#define XposL 0x04 //The lower 8 bits of the X coordinate
#define YposH 0x05 //High 4 digits of Y coordinate
#define YposL 0x06 //The lower 8 bits of the Y coordinate

#define BPC0H 0xB0 //High 8 bits of BPC0 value
#define BPC0L 0xB1 //The lower 8 bits of the BPC0 value
#define BPC1H 0xB2 //The upper 8 bits of the BPC1 value
#define BPC1L 0xB3 //The lower 8 bits of the BPC1 value
#define ChipID 0xA7 //Chip model
#define ProjID 0xA8 //Project Number

#define FwVersion 0xA9 //software version number

#define MotionMask 0xEC
#define EnConLR  0x04 //Enable continuous left and right swipe actions
#define EnConUD  0x02 //Enable continuous up and down sliding action
#define EnDClick 0x01 //Enable double-click action

#define IrqPluseWidth 0xED //Interrupt low pulse output width unit 0.1ms, optional value: 1~200, default value is 10

#define NorScanPer 0xEE //Normal fast detection cycle unit 10ms, optional value: 1~30, default value is 1

#define MotionSlAngle 0xEF //Gesture detection sliding partition angle control Angle=tan(c)*10
							//c is the angle based on the positive direction of the x-axis
#define LpScanRaw1H 0xF0 //Low power consumption scans the upper 8 bits of the reference value of channel 1
#define LpScanRaw1L 0xF1 //Low power scan the lower 8 bits of the reference value of channel 1
#define LpScanRaw2H 0xF2 //Low power consumption scans the upper 8 bits of the reference value of channel 1
#define LpScanRaw2L 0xF3 //Low power scan the lower 8 bits of the reference value of channel 1

#define LpAutoWakeTime 0xF4 //Auto-recalibration cycle at low power consumption Unit: 1 minute, optional value: 1~5. The default value is 5
#define LpScanTH 0xF5 //Low power scan wake-up threshold. The smaller the value, the more sensitive it is. Available values: 1-255. The default value is 48
#define LpScanWin 0xF6 //Low power scan range. The larger the value, the more sensitive and the higher the power consumption. Available values: 0, 1, 2, 3. The default value is 3
#define LpScanFreq 0xF7 //Low power consumption scan frequency The smaller the more sensitive the optional value: 1~255. The default value is 7
#define LpScanIdac 0xF8 //Low power consumption scanning current The smaller the more sensitive Optional value: 1～255
#define AutoSleepTime 0xF9 //When there is no touch within x seconds, it will automatically enter the low power consumption mode. The unit is 1S, and the default value is 2S.

#define IrqCtl 0xFA
#define EnTest 0x80 //Interrupt pin test, automatically sends out low pulses periodically after enabling
#define EnTouch 0x40 //When a touch is detected, periodically emit a low pulse
#define EnChange 0x20 //Sends a low pulse when a touch state change is detected
#define EnMotion 0x10 //When a gesture is detected, emit a low pulse
#define OnceWLP 0x00 //The long press gesture only sends out a low pulse signal

#define AutoReset 0xFB //When there is a touch but no valid gesture within x seconds, it will automatically reset. The unit 1S is not enabled when it is 0. The default is 5.
#define LongPressTime 0xFC //Press and hold for x seconds to reset automatically. Unit 1S is not enabled when it is 0. The default is 10.
#define IOCtl 0xFD //
#define SOFT_RST 0x04 //Enable soft reset
#define IIC_OD 0x02 //OD
#define En1v8  0x01 //1.8V

#define DisAutoSleep 0xFE //The default is 0, enabling automatic entry into low power consumption mode; non-zero value, prohibiting automatic entry into low power consumption mode

typedef struct{
	uint8_t mode;
	uint8_t Gesture;
	uint16_t color;
	uint16_t x_point;
	uint16_t y_point;
}cst816t_XY;

extern cst816t_XY XY;

typedef enum {
    TOUCH_INIT  = 0,
    TOUCH_IRQ,
    TOUCH_FUNCTION,
	TOUCH_DRAW,
	TOUCH_OUT_GESTURE,
	TOUCH_NO_DRAW,
} Touch_STATE;


uint8_t cst816t_init(uint8_t mode);
// void cst816t_Reset(void);

cst816t_XY cst816t_Get_Point(void);  // プロトタイプの修正

// uint8_t read_finger_num(void);

// cst816t_XY cst816t_Get_Point();

