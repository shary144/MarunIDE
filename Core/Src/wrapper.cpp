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

extern ADC_HandleTypeDef hadc1;

//とりあえずモーターの回転とかは先に宣言して後からオーバーライド
int mortor(int rotatev/*-100%~100%*/,int id){
	if ((-100<rotatev) and (rotatev<100)){

	}else{/*範囲外*/};

	if ((-180<arg) and (arg<180)){

	}else{/*範囲外*/};

	return 0;
}
int handle(int v,int arg/*直進が0,反時計回りに増える*/){
	/*正射影ベクトルを取るとか*/
}
int main(){
	return 0;
}
