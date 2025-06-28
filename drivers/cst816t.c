// Copyright 2025 mass
// SPDX-License-Identifier: GPL-2.0-or-later

#include "cst816t.h"
#include "../config.h"
#include "i2c_master.h"
#include "gpio.h"
#include "wait.h"

uint8_t cst816t_WhoAmI(void) {
    uint8_t who_am_i = 0;
    if (i2c_readReg(CST816S_ADDR, ChipID, &who_am_i, 1, 1000) == I2C_STATUS_SUCCESS) {
        if (who_am_i == 0xB5) {  
            return true;
        }
    }
    return false;
}

void cst816t_Reset(void) {
    setPinOutput(TOUCH_RST_PIN); 
    writePinLow(TOUCH_RST_PIN);
    wait_ms(100);
    writePinHigh(TOUCH_RST_PIN);
    wait_ms(100);
}

uint8_t cst816t_Read_Revision(void) {
    uint8_t rev = 0;
    if (i2c_readReg(CST816S_ADDR, FwVersion, &rev, 1, 1000) == I2C_STATUS_SUCCESS) {
        return rev;
    }
    return 0;
}

void cst816t_Stop_Sleep(void) {
    uint8_t stop_sleep_cmd = 0x01;
    i2c_writeReg(CST816S_ADDR, 0xFA, &stop_sleep_cmd, 1, 1000);
}

void cst816t_Set_Mode(uint8_t mode) {
    if (mode == 1) {
        uint8_t data = 0x41;
        i2c_writeReg(CST816S_ADDR, IrqCtl, &data, 1, 1000);
        uint8_t nor_scan_per = 0x01;
        i2c_writeReg(CST816S_ADDR, NorScanPer, &nor_scan_per, 1, 1000);
        uint8_t irq_pulse_width = 0x01;
        i2c_writeReg(CST816S_ADDR, IrqPluseWidth, &irq_pulse_width, 1, 1000);
    } else if (mode == 2) {
        uint8_t data = 0x71;
        i2c_writeReg(CST816S_ADDR, IrqCtl, &data, 1, 1000);
        uint8_t nor_scan_per = 0x01;
        i2c_writeReg(CST816S_ADDR, NorScanPer, &nor_scan_per, 1, 1000);
        uint8_t irq_pulse_width = 0x01;
        i2c_writeReg(CST816S_ADDR, IrqPluseWidth, &irq_pulse_width, 1, 1000);
    } else {
        uint8_t data = 0x11;
        i2c_writeReg(CST816S_ADDR, IrqCtl, &data, 1, 1000);
    }
}



void cst816t_Wake_up(void) {
    setPinOutput(TOUCH_RST_PIN);
    writePinLow(TOUCH_RST_PIN);
    wait_ms(10);
    writePinHigh(TOUCH_RST_PIN);
    wait_ms(50);
    uint8_t data = 0x01;
    i2c_writeReg(CST816S_ADDR, 0xFE, &data, 1, 1000);
}

uint8_t cst816t_init(uint8_t mode) {
    uint8_t bRet, Rev;
    cst816t_Reset();
    bRet = cst816t_WhoAmI();
    if (bRet) {
        printf("Success: Detected CST816T.\r\n");
        Rev = cst816t_Read_Revision();
        printf("CST816T Revision = %d\r\n", Rev);
        cst816t_Stop_Sleep();
    } else {
        printf("Error: Not Detected CST816T.\r\n");
        return false;
    }
    XY.mode = mode;
    cst816t_Set_Mode(mode);
    XY.x_point = 0;
    XY.y_point = 0;
    return true;
}

cst816t_XY XY;
cst816t_XY cst816t_Get_Point() {
    uint8_t data[4];
    i2c_readReg(CST816S_ADDR, 0x03, data, 4, 1000);

    XY.x_point = ((data[0] & 0x0f)<<8) + data[1];
    XY.y_point = ((data[2] & 0x0f)<<8) + data[3];

    uint8_t gesture_data;
    i2c_readReg(CST816S_ADDR, 0x01, &gesture_data, 1, 1000);
    XY.Gesture = gesture_data;

    return XY;
}


// add

bool get_finger_present(bool prev_state) {
    uint8_t finger;
    if (i2c_readReg(CST816S_ADDR, 0x02, &finger, 1, 1000) == I2C_STATUS_SUCCESS) {
        return (finger & 0x0F) != 0;   // 1→触れている, 0→離れている
    }
    return 0;                 // 読み取り失敗時は前回値
}










