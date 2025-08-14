#ifndef SPI_SLAVE_HPP
#define SPI_SLAVE_HPP

#pragma once

#include "main.h"
#include <stdint.h>
#include <stm32f4xx_hal_spi.h>
#include <stm32f446xx.h>
#include "LED.hpp"
#include "UART_Handler.hpp"
#include <cstdint>

class SPI_Slave
{
    private:
        SPI_HandleTypeDef* hspi;

        uint8_t* txBuffer;
        uint8_t* rxBuffer;
        uint16_t bufferSize;

        LED* led; // LEDクラスのインスタンス
        UART_Handler* uart; // UARTハンドラのポインタ

        void blinkLED(int times, int delayMs);
    public:
        SPI_Slave(SPI_HandleTypeDef* hspi, UART_Handler* uart, LED* led);
        void beginReceiveTransmit(uint8_t* txData, uint8_t* rxData, uint16_t size);

        // 割り込みコールバックから呼ぶ関数
        void onReceiveTransmitComplete_Callback();
        void slaveError_Callback();
};

#endif // SPI_SLAVE_HPP