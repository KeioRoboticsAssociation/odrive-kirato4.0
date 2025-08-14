#ifndef LED_HPP
#define LED_HPP

#pragma once

#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stm32f446xx.h>


class LED
{
private:
    GPIO_TypeDef *ledPort;
    uint16_t myledPin;
public:
    LED(GPIO_TypeDef *ledport, uint16_t myledPin);
    void toggle();
    void on();
    void off();
};


#endif // LED_HPP
