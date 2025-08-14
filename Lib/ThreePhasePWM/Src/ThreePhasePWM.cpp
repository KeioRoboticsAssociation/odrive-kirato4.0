#include "ThreePhasePWM.hpp"

ThreePhasePWM::ThreePhasePWM(TIM_HandleTypeDef* htim, uint32_t channel_U, uint32_t channel_V, uint32_t channel_W)
{
    this->htim = htim;
    this->channel_U = channel_U;
    this->channel_V = channel_V;
    this->channel_W = channel_W;
    theta = 0;
}

void ThreePhasePWM::start()
{
    HAL_TIM_PWM_Start(htim, channel_U);
    HAL_TIM_PWM_Start(htim, channel_V);
    HAL_TIM_PWM_Start(htim, channel_W);
}

void ThreePhasePWM::setDuty(uint32_t channel, float duty)
{
    if (duty > 1.0f)
    {
        duty = 1.0f;
    }

    if (duty < 0.0f)
    {
        duty = 0.0f;
    }

    uint32_t pulse = static_cast<uint32_t>(htim->Init.Period * duty);
    __HAL_TIM_SET_COMPARE(htim, channel, pulse);
}
void ThreePhasePWM::update()
{
    theta = theta + 0.1f; // 進み量で光るスピードが変わる
    if (theta >= 2*M_PI)
    {
        theta = theta - 2*M_PI;
    }

    float duty1 = (50 + 50 * sin(theta)) / 100;
    float duty2 = (50 + 50 * sin(theta + 2.0f/3.0f * M_PI)) / 100;
    float duty3 = (50 + 50 * sin(theta + 4.0f/3.0f * M_PI)) / 100;

    setDuty(channel_U, duty1);
    setDuty(channel_V, duty2);
    setDuty(channel_W, duty3);
}