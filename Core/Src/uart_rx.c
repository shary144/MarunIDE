/*
 * uart_rx.c
 *
 *  Created on: Oct 15, 2025
 *      Author: 12014
 */

#include "usart.h"
#include "frame.h"

#define RX_BUF_SZ 512
static volatile uint8_t rxbuf[RX_BUF_SZ];
static volatile uint16_t head = 0; // 次に書く位置
static volatile uint16_t tail = 0; // 次に読む位置

static uint8_t rx_byte;

void uart_rx_start(void){
    HAL_UART_Receive_IT(&huart1, &rx_byte, 1);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
    if (huart->Instance == USART1) {
        rxbuf[head] = rx_byte;
        head = (head + 1) % RX_BUF_SZ;
        HAL_UART_Receive_IT(&huart1, &rx_byte, 1);
    }
}

// リングから1バイト取り出し（空なら-1）
int rb_get(void){
    if (tail == head) return -1;
    int b = rxbuf[tail];
    tail = (tail + 1) % RX_BUF_SZ;
    return b;
}

// フレーム探索して1件取り出し
int read_frame(JoyFrame* out){
    // SYNC探索
    while (1) {
        int b0 = rb_get(); if (b0 < 0) return 0; // not ready
        if (b0 != 0x55) continue; // LSB of 0xAA55
        int b1 = rb_get(); if (b1 < 0) { tail = (tail + RX_BUF_SZ - 1) % RX_BUF_SZ; return 0; }
        if (b1 != 0xAA)   continue;
        // 残り22バイト必要
        uint8_t rest[FRAME_SIZE - 2];
        for (int i=0;i<FRAME_SIZE-2;i++){
            int bi = rb_get(); if (bi < 0) { // 足りない→戻す
                // 戻す（雑だが安全策：今回の読みを全部戻す）
                tail = (tail + RX_BUF_SZ - 2 - i) % RX_BUF_SZ;
                return 0;
            }
            rest[i] = (uint8_t)bi;
        }
        JoyFrame* f = (JoyFrame*)out;
        f->sync     = 0xAA55;
        f->ver      = rest[0];
        f->reserved = rest[1];
        // 残りをmemcpy
        memcpy(((uint8_t*)out)+4, &rest[2], FRAME_SIZE-4);

        if (f->ver != FRAME_VER) continue; // バージョン違いは捨てる

        // CRCチェック: ver..seq（20バイト）
        uint16_t calc = crc16_ccitt(&rest[0], 20);
        if (calc != f->crc16) {
            continue; // 破損
        }
        return 1; // 1件OK
    }
}


