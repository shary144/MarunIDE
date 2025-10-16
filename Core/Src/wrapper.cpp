/*
 * wrapper.cpp
 *
 *  Created on: Aug 18, 2025
 *      Author: 12014
 */
//some change
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "uart_rx.h"
#include <cstdio>
#include <cstring>
#include "frame.h"
#include <cmath> //
double pi=3.141592;

int center[2] = {2,0};
//とりあえずモーターの回転とかは先に宣言して後からオーバーライド
int motor(int rotatev, int id){
    if (rotatev < -100 || rotatev > 100) return -1;

    // PWM変換処理（仮）
    int pwm = (rotatev + 100) * 10; // 0〜2000の範囲に変換など

    // idに応じてTIMチャネルに出力（例）
    switch(id){
        case 0: __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, pwm); break;
        case 1: __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, pwm); break;
        case 2: __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, pwm); break;
        default: return -2;
    }

    return 0;
}

struct UnderCarriage{
	using namespace std;
	int init_ang = 0;
	int rcenter[2]={2,0};//{distance,arg}
	void handle(int v,int angv/*直進が0,反時計回りに増える*/){
		/*正射影ベクトルを取るとか*/
		double x = v*cmath.cos(angv*pi/180);
		double y = v*cmath.sin(angv*pi/180);
		for(int id=0;id<3;id++){
			int rotatev=-cmath.sin((init_ang+120*id)*pi/180)*x+cmath.cos((init_ang+120*id)*pi/180)*y;
			motor(rotatev,id);
		}
	}
	void rotate(int rotatev){
		//ここは専ら向きのひずみの修正か射出のための機体転回か
		for(int id=0;id<3;id++){
			motor(rotatev,id);
		}
	}
};

extern "C" void main_cpp() {
	uart_rx_start();

	JoyFrame jf;

    while (1) {
  	  if (read_frame(&jf)) {
  		float LX = q15_to_f(jf.lx_q15);   // -1から1
  		float LY = q15_to_f(jf.ly_q15);   // -1から1
  		float RX = q15_to_f(jf.rx_q15);   // -1から1
  		float RY = q15_to_f(jf.ry_q15);   // -1から1
  		float LT = q15_to_f01(jf.lt_q15); // 0から1
  		float RT = q15_to_f01(jf.rt_q15); // 0から1
  		uint16_t btn = jf.btn_mask;       // シフト演算子を使ってデータを取得しよう
  		int8_t hx = jf.hat_x, hy = jf.hat_y;

  		if (btn & (1 << 0)) {  // A
  		    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET); // 点灯
  		} else {
  		    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);   // 消灯
  		}
  		if (btn & (1 << 1)) {  // B
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET); // 点灯
		} else {
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET);   // 消灯
		}
  		if (btn & (1 << 2)) {  // X
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET); // 点灯
		} else {
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET);   // 消灯
		}
        //僕が来ました
  	  }

    }
}
