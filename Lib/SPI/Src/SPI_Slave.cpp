#include "SPI_Slave.hpp"
#include "UART_Handler.hpp"
#include "LED.hpp"
#include <cstdio>

SPI_Slave::SPI_Slave(SPI_HandleTypeDef* hspi, UART_Handler* uart, LED* led)    
{
    this->hspi = hspi;
    this->uart = uart; // UARTハンドラのポインタを初期化
    this->led = led; // LEDクラスのインスタンスを初期化

    // 受信バッファと送信バッファの初期化
    this->txBuffer = nullptr;
    this->rxBuffer = nullptr;
    this->bufferSize = 0;
}

void SPI_Slave::beginReceiveTransmit(uint8_t* txData, uint8_t* rxData, uint16_t size)
{
    txBuffer = txData;
    rxBuffer = rxData;
    bufferSize = size;

    // SPI受信送信を開始
    HAL_SPI_TransmitReceive_IT(hspi, txBuffer, rxBuffer, bufferSize);
}

void SPI_Slave::blinkLED(int times, int delayMs)
{
    for (int i = 0; i < times; ++i)
    {
        led->on();
        HAL_Delay(delayMs);
        led->off();
        HAL_Delay(delayMs);
    }
}

void SPI_Slave::onReceiveTransmitComplete_Callback()
{
    char msg[50];
    snprintf(msg, sizeof(msg), "SPI受送信完了: Tx=%02X Rx=%02X\n", txBuffer[0], rxBuffer[0]);
    uart->sendMessage(msg); // UARTでメッセージを送信
    blinkLED(3,200); // LEDを3回点滅
}

void SPI_Slave::slaveError_Callback()
{
    uart->sendMessage("SPIスレーブエラー発生\r\n");
}

//(rxData[0] << 8) | rxData[1]; 