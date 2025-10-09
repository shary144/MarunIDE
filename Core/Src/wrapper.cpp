/*
 * wrapper.cpp
 *
 *  Created on: Aug 18, 2025
 *      Author: 12014
 */
#include "main.h"
#include "tim.h"
#include "adc.h"

extern ADC_HandleTypeDef hadc1;

// センサ値保持用配列
uint32_t adc_values[4] = {0};

// 各チャンネルのしきい値（必要に応じて調整)
const uint32_t thresholds[4] = {2048, 2048, 2048, 2048};

int right_moter(int speed){
	if(speed > 100) speed=100;
	if(speed < -100) speed=-100;
	int PWM = 1500 + (2.5*speed);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, PWM);
	return 0;
}

int left_moter(int speed){
	if(speed > 100) speed=100;
	if(speed < -100) speed=-100;
	int PWM = 1500 - (2.5*speed);
	__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, PWM);
	return 0;
}

extern "C" void main_cpp()
{

    ADC_ChannelConfTypeDef sConfig = {0};

    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);   // PA6
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);   // PB6

    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 1500);   // TIM3_CH1のパルス幅を1.5msに設定
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 1500);   // TIM4_CH1のパルス幅を1.5msに設定

    HAL_Delay(1000);

    int right_speed = 0;
    int left_speed = 0;
    int stop_frag = 0;

    while (1)
    {
    	stop_frag = 0;
    	for (int i = 0; i < 4; i++)
    	{
    	    sConfig.Channel = ADC_CHANNEL_0 + i;
    	    sConfig.Rank = ADC_REGULAR_RANK_1;
    	    sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES_5;

    	    HAL_ADC_ConfigChannel(&hadc1, &sConfig);

    	    HAL_ADC_Start(&hadc1);
    	    if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK)
    	    {
    	        adc_values[i] = HAL_ADC_GetValue(&hadc1);
    	    }
    	    HAL_ADC_Stop(&hadc1);
    	}

        if (adc_values[0] >= thresholds[0]) {
        	if (adc_values[2] >= thresholds[2]){
        		stop_frag=1;
        	}
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
            //白以外(道の上)
            left_speed = 100;
            right_speed = -50;
        } else {
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
            //白.
        }
        if (adc_values[1] >= thresholds[1]) {
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
            left_speed = 100;
            right_speed = 50;
        } else {
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
        }
        if (adc_values[2] >= thresholds[2]) {
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
            right_speed = 100;
            left_speed = 50;
        } else {
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
        }
        if (adc_values[3] >= thresholds[3]) {
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
            right_speed = 100;
            left_speed = -50;
            stop_frag = 0;
        } else {
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
        }
        if (stop_frag==1){
        	right_speed=-100;
        	left_speed =100;
        }
        right_moter(right_speed);
        left_moter(left_speed);

    }
}
