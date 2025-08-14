#include "PWMController.hpp"

#include <stm32f4xx_hal.h>
#include <stm32f4xx_hal_tim.h>
#include <stm32f446xx.h>

#include <cstdint>

#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

PWMController::PWMController(TIM_HandleTypeDef* htim, uint32_t channel, uint32_t timClockHz){
    this->htim = htim;
    this->channel = channel;
    this->timClockHz = timClockHz;
    // タイマーのクロック周波数を取得
    cmax = __HAL_TIM_GET_AUTORELOAD(htim);
    cmax = 0;
}

// タイマのARRを設定し、PWM周波数を決める
void PWMController::setPWMFrequency(uint32_t frequency)
{
    // 周波数からカウント値を計算
    uint32_t count = timClockHz / frequency;
    __HAL_TIM_SET_AUTORELOAD(htim, count - 1);
    __HAL_TIM_SET_COUNTER(htim, 0); // カウンターをリセット
    cmax = count - 1; // 更新されたカウント値を保存
}

// 任意の3相デューティ比をCCRに書く
void PWMController::updateDutyCycles(float duty1, float duty2, float duty3)
{
    // デューティ比をパルス幅に変換
    uint32_t pulse1 = static_cast<uint32_t>(cmax * duty1);
    uint32_t pulse2 = static_cast<uint32_t>(cmax * duty2);
    uint32_t pulse3 = static_cast<uint32_t>(cmax * duty3);

    // パルス幅を設定
    __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1, pulse1);
    __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2, pulse2);
    __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_3, pulse3);
}

// サイン波形を生成し、updateDutyCycles に渡す
void PWMController::updateSineDutyFromTime(double timeSec, double omegaHz)
{
    // サイン波のデューティ比を計算
    float duty1 = 0.5f * (1.0f + sin(2.0f * M_PI * omegaHz * timeSec));
    float duty2 = 0.5f * (1.0f + sin(2.0f * M_PI * omegaHz * timeSec + 2.0f * M_PI / 3.0f));
    float duty3 = 0.5f * (1.0f + sin(2.0f * M_PI * omegaHz * timeSec + 4.0f * M_PI / 3.0f));

    // デューティ比を更新
    updateDutyCycles(duty1, duty2, duty3);
}