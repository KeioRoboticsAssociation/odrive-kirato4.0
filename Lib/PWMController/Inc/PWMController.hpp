#ifndef PWM_CONTROLLER_HPP
#define PWM_CONTROLLER_HPP

#pragma once

#include "main.h"
#include <stdint.h>
#include <stm32f4xx_hal_tim.h>
#include <stm32f446xx.h>
#include "LED.hpp"

class PWMController
{
private:
    TIM_HandleTypeDef* htim; // タイマーのハンドル
    uint32_t channel;        // PWMチャネル
    uint32_t timClockHz;     // イマークロック周波数
    uint32_t cmax;          // パルス幅
public:
    PWMController(TIM_HandleTypeDef* htim, uint32_t channnel, uint32_t timClockHz);

    void setPWMFrequency(uint32_t frequency); // PWM周波数を設定
    void updateDutyCycles(float duty1, float duty2, float duty3); // デューティ比を更新
    void updateSineDutyFromTime(double timeSEc, double omegaHz); // サイン波のデューティ比を更新
};

#endif // PWM_CONTROLLER_HPP