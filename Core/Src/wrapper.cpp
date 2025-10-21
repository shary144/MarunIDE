/*
 * wrapper.cpp
 *
 *  Created on: Aug 18, 2025
 *      Author: 12014
 */
//some change?
//動きませんでした！！！！！！！(10/20)
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "uart_rx.h"
#include <cstdio>
#include <cstring>
#include "frame.h"
<<<<<<< HEAD
#include <cmath>
=======
#include <cmath> 
>>>>>>> f68427346082e7be5abf2b158198d02048f35791
const double pi=3.141592;

using namespace std;

<<<<<<< HEAD
//ライントレーサの時と同じ仕組みだとしたらこれ.
//関数motorに、どのくらい回転させるか(speed(-100から100)で指定)、どの車輪を回転させるか(id(0,1,2で指定)送る)を指示する
int moter(int speed,int id){
	if(speed > 100) speed=100;
	if(speed < -100) speed=-100;
	//iocファイルの"Core" > GPIO_OUTPUT : Configuration > PWM から使っているピンの一覧が見れます
	if (speed < 0) {
		speed = -speed;
		switch(id){
		  case 0:
			  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, speed);
			  HAL_GPIO_WritePin(DC_DIR_1_GPIO_Port,DC_DIR_1_Pin,GPIO_PIN_RESET);
			  break;
		  case 1:
			  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, speed);
			  HAL_GPIO_WritePin(DC_DIR_2_GPIO_Port,DC_DIR_2_Pin,GPIO_PIN_RESET);
			  break;
		  case 2:
			  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, speed);
			  HAL_GPIO_WritePin(DC_DIR_3_GPIO_Port,DC_DIR_3_Pin,GPIO_PIN_RESET);
			  break;
		  default:
			  return -1;
		}
	} else {
		switch(id){
		  case 0:
			  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, speed);
			  HAL_GPIO_WritePin(DC_DIR_1_GPIO_Port,DC_DIR_1_Pin,GPIO_PIN_SET);
			  break;
		  case 1:
			  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, speed);
			  HAL_GPIO_WritePin(DC_DIR_2_GPIO_Port,DC_DIR_2_Pin,GPIO_PIN_SET);
			  break;
		  case 2:
			  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, speed);
			  HAL_GPIO_WritePin(DC_DIR_3_GPIO_Port,DC_DIR_3_Pin,GPIO_PIN_SET);
			  break;
		  default:
			  return -1;
=======
int center[2] = {2,0};
//とりあえずモーターの回転とかは先に宣言して後からオーバーライド

//関数motorに、どのくらい回転させるか(rotatev(-100から100)で指定)、どの車輪を回転させるか(id(0,1,2で指定)送る)を指示する
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
	int init_ang = 0;
	int rcenter[2]={2,0};//{distance,arg}
	void handle(int v,int angv/*直進が0,反時計回りに増える*/){
		/*正射影ベクトルを取るとか*/
		double x = v*cos(angv*pi/180);
		double y = v*sin(angv*pi/180);
		for(int id=0;id<3;id++){
			int rotatev=-sin((init_ang+120*id)*pi/180)*x+cos((init_ang+120*id)*pi/180)*y;
			motor(rotatev,id);
>>>>>>> f68427346082e7be5abf2b158198d02048f35791
		}
	}
	return 0;
}

//どれがどこに対応してるかわからないので基盤読むか実際に動かすかして
//idがどこに対応するかみてint moter(int id)内のidの振り分けを組み替えてね
int check_moter(int id){
	moter(100,id);
	return 0;
}

//足回りを制御する構造体
//車輪の回転速度には限りがあるのでリソースの管理のために回転運動と並進運動を制御できるよう構造体として用意
class UnderCarriage{
   private:
	//moter_speed[id]として各モーターの回転速度を格納する配列
	int moter_speeds[3];

	//車輪の回転速度のデータを初期化
	void init(){
		moter_speeds[0]=0;
		moter_speeds[1]=0;
		moter_speeds[2]=0;
	}
	//機体を回転させる
	void rotate(
		double rotatep,
		/*機体を回転させる速度のフルパワーからみた割合(-1.0~1.0)
		 *反時計回りが正方向*/

		double outer_c
		/*もしボールが接地した状態で動かしたり拾ったりするとき
	  	  ボールが逃げないようにするため回転の中心をボール側に持ってくるが、
	  	  そのときの((回転中心から機体の中心までの距離)/車軸長)をouter_cと定義*/
		)
	{
		for (int id=0;id<3;id++) {
			//計算式はgDriveにあげてるから見て
			//一応整数*小数は小数なので計算途中で切り捨て算は生じてないはず（と思いたい）
			int regular_rotation = (2-5*outer_c*cos((60+120*id)*pi/180))/2/(1+outer_c);
			moter_speeds[id] = (int)(100*regular_rotation*rotatep);
		}
	}
	//機体を前に進ませる
	void advance(
		double vp
		/*前方に進む速度のフルパワーから見た割合(-1.0~1.0)
		  前方が正方向*/)
	{
		// *ここでは回転運動に費やしたモーターリソースの残りを利用する
		//(つまりフルパワーで回転すると機体は前に出れない
		// 回転速度によっては安全マージンとして働くが今回はそうでないはずなので最低限度の速度は出せた方がいいかも)
		// ここまでmoter_speeds[0]=moter_speeds[2]であり、moter_speedsで最も大きい値はmoter_speeds[1]だが
		// moter1は直進運動では必ず0であるからして...説明が面倒になったのでgDriveの資料に気が向いたらあげておく
		int max_speed = 100-abs(moter_speeds[0]);
		moter_speeds[0]-=(int)(max_speed*vp);
		moter_speeds[2]+=(int)(max_speed*vp);//あんま高尚なことできなかったや
	}

   public:
	void handleBody(double straightp,double rotatep, double outer_c){
		init();
		rotate(rotatep,outer_c);
		advance(straightp);
		for(int id=0;id<3;id++){
<<<<<<< HEAD
			moter(moter_speeds[id],id);
=======
			motor(rotatev,id); //すべてのオムニホイールに同じ回転速度(rotatev）を送る
>>>>>>> f68427346082e7be5abf2b158198d02048f35791
		}
	}
};

extern "C" void main_cpp() {
	uart_rx_start();

	JoyFrame jf; //これはおそらく先輩が作った別ファイルの構造体
	UnderCarriage uc; //こっちは自作


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

  		//すまん右手は使わせてもらう
  		uc.handleBody(RX,RY,RT);

  		if (btn & (1 << 0)) {  // A
  		    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET); // 点灯
  		    check_moter(0);
  		} else {
  		    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);   // 消灯
  		}
  		if (btn & (1 << 1)) {  // B
			check_moter(1);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET); // 点灯
		} else {
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET);   // 消灯
		}
  		if (btn & (1 << 2)) {  // X
  			check_moter(2);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET); // 点灯
		} else {
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET);   // 消灯
		}
  	  }

    }
}
