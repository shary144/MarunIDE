/*
 * wrapper.cpp
 *
 *  Created on: Aug 18, 2025
 *      Author: 12014
 */
//some change?
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "uart_rx.h"
#include <cstdio>
#include <cstring>
#include "frame.h"
#include <cmath> //
const double pi=3.141592;

using namespace std;
//もしボールが接地した状態で動かしたり拾ったりするときボールが逃げないようにするため
//外側に回転中心を取りたい
int center[2] = {2,0};

//ライントレーサの時と同じ仕組みだとしたらこれ
//関数motorに、どのくらい回転させるか(speed(-100から100)で指定)、どの車輪を回転させるか(id(0,1,2で指定)送る)を指示する
int moter(int speed,int id){
	if(speed > 100) speed=100;
	if(speed < -100) speed=-100;
	int PWM = 1500 + (2.5*speed);
	//iocファイルの"Core" > GPIO_OUTPUT : Configuration > PWM から使っているピンの一覧が見れます
	if (id==0){
		//ここはtim3のchannel1にPWMの制御を書き込むという意味;　他も同様に
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, PWM);
	} else if (id==1){
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, PWM);
	} else if (id==2){
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, PWM);
	} else if (id==3){
		__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, PWM);
	}
	return 0;
}

//どれがどこに対応してるかわからないので基盤読むか実際に動かすかして
//idがどこに対応するかみてint moter(int id)内のidの振り分けを組み替えてね
int check_moter(int id){
		moter(100,id);
}

//足回りを制御する構造体
struct UnderCarriage{
	int init_ang = 0;
	int rcenter[2]={2,0};//{distance,arg}
	//機体本体の舵取り
	void handle(int v,int angv/*：直進が0,反時計回りに増える*/){
		/*正射影ベクトルを取るとか*/
		double x = v*cos(angv*pi/180);
		double y = v*sin(angv*pi/180);
		for(int id=0;id<3;id++){
			int speed=-sin((init_ang+120*id)*pi/180)*x+cos((init_ang+120*id)*pi/180)*y;
			motor(speed,id);
		}
	}
	void rotate(int rotatev){
		//ここは専ら向きのひずみの修正か射出のための機体転回
		for(int id=0;id<3;id++){
			motor(rotatev,id);
		}
	}
};

extern "C" void main_cpp() {
	uart_rx_start();

	JoyFrame jf; //これはおそらく先輩が作った別ファイルの構造体

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
