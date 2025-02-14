// Copyright 2025 mass
// SPDX-License-Identifier: GPL-2.0-or-later

#include "cst816t.h"
#include "../config.h"
#include "i2c_master.h"
#include "gpio.h"
#include "wait.h"

// デバイスIDの読み取り
uint8_t cst816t_WhoAmI(void) {
    uint8_t who_am_i = 0;
    if (i2c_readReg(CST816S_ADDR, ChipID, &who_am_i, 1, 1000) == I2C_STATUS_SUCCESS) {
        if (who_am_i == 0xB5) {  // 0xB5はCST816TのデバイスID
            return true;
        }
    }
    return false;
}

// タッチデバイスをリセット
void cst816t_Reset(void) {
    setPinOutput(TOUCH_RST_PIN);  // TOUCH_RST_PINはconfig.hなどに定義
    writePinLow(TOUCH_RST_PIN);
    wait_ms(100);  // 100ms待機
    writePinHigh(TOUCH_RST_PIN);
    wait_ms(100);  // 100ms待機
}

// ソフトウェアバージョン番号を読み取る
uint8_t cst816t_Read_Revision(void) {
    uint8_t rev = 0;
    if (i2c_readReg(CST816S_ADDR, FwVersion, &rev, 1, 1000) == I2C_STATUS_SUCCESS) {
        return rev;
    }
    return 0;
}

// スリープモードを解除
void cst816t_Stop_Sleep(void) {
    uint8_t stop_sleep_cmd = 0x01;  // スリープ解除のコマンド (仮)
    i2c_writeReg(CST816S_ADDR, 0xFA, &stop_sleep_cmd, 1, 1000);  // レジスタ0xFAに書き込む
}

void cst816t_Set_Mode(uint8_t mode) {
    if (mode == 1) {
        // ポイントモード
        uint8_t data = 0x41;
        i2c_writeReg(CST816S_ADDR, IrqCtl, &data, 1, 1000);

        uint8_t nor_scan_per = 0x01;  // 高速検出周期
        i2c_writeReg(CST816S_ADDR, NorScanPer, &nor_scan_per, 1, 1000);
        uint8_t irq_pulse_width = 0x01;  // 割り込みパルス幅
        i2c_writeReg(CST816S_ADDR, IrqPluseWidth, &irq_pulse_width, 1, 1000);

    } else if (mode == 2) {
        // ミックスモード
        uint8_t data = 0x71;
        i2c_writeReg(CST816S_ADDR, IrqCtl, &data, 1, 1000);

        uint8_t nor_scan_per = 0x01;  // 高速検出周期
        i2c_writeReg(CST816S_ADDR, NorScanPer, &nor_scan_per, 1, 1000);
        uint8_t irq_pulse_width = 0x01;  // 割り込みパルス幅
        i2c_writeReg(CST816S_ADDR, IrqPluseWidth, &irq_pulse_width, 1, 1000);
        // uint8_t motion_mask = EnDClick;  // ダブルクリックジェスチャーを有効化
        // i2c_writeReg(CST816S_ADDR, MotionMask, &motion_mask, 1, 1000);

    } else {
        // ジェスチャーモード
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
        cst816t_Stop_Sleep();  // スリープ解除
    } else {
        printf("Error: Not Detected CST816T.\r\n");
        return false;
    }

    // モードの設定
    XY.mode = mode;
    cst816t_Set_Mode(mode);
    XY.x_point = 0;
    XY.y_point = 0;
    return true;
}

// Get  coordinates
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












