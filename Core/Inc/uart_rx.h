/*
 * uart_rx.h
 *
 *  Created on: Oct 15, 2025
 *      Author: 12014
 */
#pragma once
#include <stdint.h>
#include "frame.h"

#ifdef __cplusplus
extern "C" {
#endif

void uart_rx_start(void);
int  read_frame(JoyFrame* out);
int  rb_used(void);     // あるなら宣言しておく

#ifdef __cplusplus
}
#endif
