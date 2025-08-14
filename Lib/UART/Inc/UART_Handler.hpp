#ifndef UART_HANDLER_HPP
#define UART_HANDLER_HPP

#pragma once

#include "main.h"
#include <string.h>
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_uart.h"

class UART_Handler
{
    private:
        UART_HandleTypeDef* huart;  // UARTハンドル
    public:
        UART_Handler(UART_HandleTypeDef* huart);
        void sendMessage(const char *msg); // 
};

#endif // UART_HANDLER_HPP