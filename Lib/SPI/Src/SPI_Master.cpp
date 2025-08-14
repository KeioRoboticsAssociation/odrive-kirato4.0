#include "SPI_Master.hpp"
#include "LED.hpp"
#include "UART_Handler.hpp"
#include <cstdio>

SPI_Master::SPI_Master(SPI_HandleTypeDef* hspi, GPIO_TypeDef* csPort, uint16_t csPin, UART_Handler* uart)
{    
    this->hspi = hspi;
    this->uart = uart; // UARTハンドラのポインタを初期化
    this->csPort = csPort;
    this->csPin = csPin;

    this->txBuffer = nullptr;
    this->rxBuffer = nullptr;
    this->bufferSize = 0;
}

void SPI_Master::beginTransmitReceive(uint8_t* txData, uint8_t* rxData, uint16_t size)
{
    txBuffer = txData;
    rxBuffer = rxData;
    bufferSize = size;

    HAL_GPIO_WritePin(csPort, csPin, GPIO_PIN_RESET); // CS LOW

    // 割り込み送受信開始
    HAL_SPI_TransmitReceive_IT(hspi, txBuffer, rxBuffer, bufferSize);
}

void SPI_Master::onTransmitReceiveComplete_Callback()
{
    HAL_GPIO_WritePin(csPort, csPin, GPIO_PIN_SET); // CS HIGH
    hspi->State = HAL_SPI_STATE_READY; // 状態をREADYに戻す
    hspi->ErrorCode = HAL_SPI_ERROR_NONE; // エラーコードをリセット
    // 送受信完了時の処理
    if (hspi->State == HAL_SPI_STATE_READY) {
        // 送受信データをUARTで送信
        char msg[50];
        snprintf(msg, sizeof(msg), "SPI送受信完了: Tx=%02X Rx=%02X\n", txBuffer[0], rxBuffer[0]);
        uart->sendMessage(msg);
    }
}

void SPI_Master::masterError_Callback()
{
    // エラー発生時の処理
    if (hspi->ErrorCode != HAL_SPI_ERROR_NONE) {
        char msg[50];
        snprintf(msg, sizeof(msg), "SPIマスターエラー: %lu\n", hspi->ErrorCode);
        uart->sendMessage(msg);
    }
}

