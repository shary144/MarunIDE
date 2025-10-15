/*
 * frame.h
 *
 *  Created on: Oct 15, 2025
 *      Author: 12014
 */
#pragma once
#include <stdint.h>

#define FRAME_SYNC 0xAA55
#define FRAME_VER  1
#define FRAME_SIZE 24

typedef struct __attribute__((packed)) {
    uint16_t sync;     // 0xAA55 (LE)
    uint8_t  ver;      // 1
    uint8_t  reserved; // 0
    int16_t  lx_q15;
    int16_t  ly_q15;
    int16_t  rx_q15;
    int16_t  ry_q15;
    uint16_t lt_q15;
    uint16_t rt_q15;
    uint16_t btn_mask;
    int8_t   hat_x;
    int8_t   hat_y;
    uint16_t seq;
    uint16_t crc16;    // CCITT over bytes [ver..seq]
} JoyFrame;

static inline float q15_to_f(int16_t q)   { return (float)q / 32767.0f; }
static inline float q15_to_f01(uint16_t q){ return (float)q / 32767.0f; }

uint16_t crc16_ccitt(const uint8_t* data, uint16_t len);
