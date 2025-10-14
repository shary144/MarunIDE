/*
 * wrapper.cpp
 *
 *  Created on: Aug 18, 2025
 *      Author: 12014
 */
//some change
#include "main.h"
#include "tim.h"
#include "adc.h"
#include <cmath>

extern ADC_HandleTypeDef hadc1;

int center[2] = {2,0};
//とりあえずモーターの回転とかは先に宣言して後からオーバーライド
int motor(int rotatev/*-100%~100%*/,int id){
	if ((-100<rotatev) and (rotatev<100)){

	}else{/*範囲外*/};

	if ((-180<arg) and (arg<180)){

	}else{/*範囲外*/};

	return 0;
}

struct HandleBody{
	int init_arg = 0;
	int rcenter[2]={2,0};//{distance,arg}
	int handle(int v,int argv/*直進が0,反時計回りに増える*/){
		/*正射影ベクトルを取るとか*/
		double x = v*cmath.cos(argv*cmath.pi/180);
		double y = v*cmath.sin(argv*cmath.pi/180);
		for(int id=0;id<3;id++){
			int rotatev=-cmath.sin((init_arg+120*id)*cmath.pi/180)*x+cmath.cos((init_arg+120*id)*cmath.pi/180)*y;
			motor(rotatev,id);
		}
	}
	int rotate(int rotatev){
		//ここは専ら向きのひずみの修正か射出のための機体転回か
		for(int id=0;id<3;id++){
			motor(rotatev,id);
		}
	}
};
int main(){
	HandleBody HB;
	/*
	 * while(1){
	 * 	うけとり
	 * }
	 *
	 */
	return 0;
}
