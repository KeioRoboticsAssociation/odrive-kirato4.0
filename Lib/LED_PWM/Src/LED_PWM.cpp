#include "LED_PWM.hpp"
#include <stdio.h>
#include <string.h>
#include "usart.h"

LED_PWM::LED_PWM(TIM_HandleTypeDef* htim, uint32_t channel)
{
    // コンストラクタ
    this->htim = htim;
    this->channel = channel;
    this->count = 0;
    // this->stage = 0;
}

void LED_PWM::start()
{
    HAL_TIM_PWM_Start(htim, channel);
}


void LED_PWM::setDuty(uint16_t percent)
{
    if (percent > 100)
    {
        percent = 100;
    }

    // ARR (= Period) に対する duty比
    uint32_t pulse = (htim->Init.Period * percent) / 100;//  uint32_tは小数を整数に変える
    __HAL_TIM_SET_COMPARE(htim, channel, pulse);  // パルス幅を設定
    // __HAL_TIM_SET_COMPARE は パルス幅（CCRの値） を直接設定するので、必ず ARR (= Period) でスケールしないといけません。
}

// void LED_PWM::update()
// {
//     // PWMテーブルを進める
//     count++;
//     if (count >= tblSize)
//     {
//         count = 0;
//         stage++;  // テーブルの次のステージへ進む
//         if (stage >= tblSize)
//         {            
//             stage = 0;  // テーブルの最後に到達したら最初に
//         }
//     }
//     // パルス幅を更新
//     setDuty(pwmTbl[count]);
    
//     // デバッグ用の出力
//     char msg[32];
//     sprintf(msg, "Duty: %d%%\r\n", pwmTbl[stage]);
//     HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
// }


void LED_PWM::update() 
{
    // 角度[rad]
    double theta = 2.0 * M_PI * count / tblSize;

    // duty比を計算 0~100%
    double duty = (std::sin(theta) + 1.0) * 50.0;  // 0〜100%

    // パルス幅を計算
    double pulse = (htim->Init.Period * duty) / 100.0;  // ARR (= Period) に対する duty比

    // CCRに書き込む
    __HAL_TIM_SET_COMPARE(htim, channel, static_cast<uint32_t>(pulse));

    // デバッグ用の出力
    count++;
    if (count >= tblSize) 
    {
        count = 0;  // カウンタをリセット
    }
}
    
