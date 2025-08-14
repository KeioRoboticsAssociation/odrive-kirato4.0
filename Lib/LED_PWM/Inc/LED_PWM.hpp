#ifndef LED_PWM_HPP
#define LED_PWM_HPP

#pragma once

#include "main.h"
#include <stdint.h>
#include <stm32f4xx_hal_tim.h>
#include <stm32f446xx.h>
#include "usart.h"
#include <cmath>  // std::sin, M_PI

// M_PIはC++11以降で定義されているため、cmathをインクルードする必要があります
// もしM_PIが定義されていない場合は、以下のように定義することもできます
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif // M_PI

class LED_PWM
{
private:
    TIM_HandleTypeDef* htim; //.cppを参照しているため、TIM_HandleTypeDefは.cppで定義されている必要があります
    uint32_t channel;
    unsigned char count; // カウンタ　0~tblSize
    static constexpr int tblSize = 100; //分割数（多いほど滑らか）
    
    // unsigned char stage;
    // static constexpr uint16_t pwmTbl[10] = {0,20,40,60,80,100,80,60,40,20};
    // static constexpr int tblSize = sizeof(pwmTbl) / sizeof(uint16_t);

public:
    LED_PWM(TIM_HandleTypeDef* htim, uint32_t channel);
    void update();  // 割り込み内で呼び出す、PWMテーブルを進める
    
    void start();
    void setDuty(uint16_t percent); // dutyを0~100で指定
};

#endif // LED_PWM_HPP