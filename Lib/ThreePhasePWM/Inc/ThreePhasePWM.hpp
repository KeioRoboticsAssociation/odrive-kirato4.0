#ifndef THREE_PHASE_PWM_HPP
#define THREE_PHASE_PWM_HPP

#pragma once

#include "main.h"
#include <stdint.h>
#include <stm32f4xx_hal_tim.h>
#include <stm32f446xx.h>
#include <cmath> // std:sin, M_PI

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif // M_PI

class ThreePhasePWM
{
    private:
        TIM_HandleTypeDef* htim;
        uint32_t channel_U;
        uint32_t channel_V;
        uint32_t channel_W;
        unsigned char count;
        static constexpr int tblSize = 100;
        float theta;
    public:
        ThreePhasePWM(TIM_HandleTypeDef* htim, uint32_t channel_U, uint32_t channel_V, uint32_t channel_W);
        void update(); // 呼び出すと位相を進める。
        void start();
        void setDuty(uint32_t channel, float duty);
};        

#endif // THREE_PHASE_PWM_HPP

// while(1){
//     HAL_ADC_Start(&hadc1);
//     HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY); // HAL_MAX_DELAY=100
//     HAL_ADC_Stop(&hadc1);
//     speed = HAL_ADC_GetValue(&hadc1) >> 2;

//     if (gate block == 1){
//         HAL_GPIO_WritePin(GPIOC, EN1_Pin, GPIO_PIN_SET);
//         HAL_GPIO_WritePin(GPIOC, EN2_Pin, GPIO_PIN_SET);
//         HAL_GPIO_WritePin(GPIOC, EN3_Pin, GPIO_PIN_SET);
//         continue;
//     }
//     H1 = HAL_GPIO_ReadPin(GPIOC, H1_Pin);
//     H2 = HAL_GPIO_ReadPin(GPIOC, H2_Pin);
//     H3 = HAL_GPIO_ReadPin(GPIOC, H3_Pin);
// }