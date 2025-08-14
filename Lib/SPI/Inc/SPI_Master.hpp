#ifndef SPI_MASTER_HPP
#define SPI_MASTER_HPP

#pragma once

#include "main.h"
#include <stdint.h>
#include <stm32f4xx_hal_spi.h>
#include <stm32f446xx.h>
#include "LED.hpp" // Include the LED class header
#include "UART_Handler.hpp" // Include the UART handler header
#include <cstdint>

class SPI_Master
{
    private:
        SPI_HandleTypeDef* hspi;
        GPIO_TypeDef* csPort;
        uint16_t csPin;

        uint8_t* txBuffer;
        uint8_t* rxBuffer;
        uint16_t bufferSize;

        //LED* statusled; // LEDクラスのインスタンス
        UART_Handler* uart; // UARTハンドラのポインタ

    public:
        SPI_Master(SPI_HandleTypeDef* hspi, GPIO_TypeDef* csPort, uint16_t csPin, UART_Handler* uart);
        void beginTransmitReceive(uint8_t* txData, uint8_t* rxData, uint16_t size);

        // 割り込みコールバックから呼ぶ関数
        void onTransmitReceiveComplete_Callback();
        void masterError_Callback();
};

#endif // SPI_MASTER_HPP