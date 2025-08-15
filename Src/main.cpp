#include "main.hpp"
#include "stm32f4xx_hal.h"
#include "LED.hpp"
#include "LED_PWM.hpp"
#include "PWMController.hpp"
#include "SPI_Master.hpp"
#include "SPI_Slave.hpp"
#include "UART_Handler.hpp"
#include "ThreePhasePWM.hpp"
#include <cstdio>

// ==== 外部ハンドル ====
extern TIM_HandleTypeDef htim1; 
extern TIM_HandleTypeDef htim2; 
extern TIM_HandleTypeDef htim7; 
extern UART_HandleTypeDef huart2;
extern SPI_HandleTypeDef hspi2;  // Master
extern SPI_HandleTypeDef hspi3;  // Slave

// ==== バッファ ====
#define SPI_LEN 2
uint8_t masterTx[SPI_LEN] = {10,20};
uint8_t masterRx[SPI_LEN] = {0};
uint8_t slaveTx[SPI_LEN]  = {1,2};
uint8_t slaveRx[SPI_LEN]  = {0};

// ==== オブジェクト ====
LED_PWM led_pwm(&htim2, TIM_CHANNEL_1);
ThreePhasePWM phaseUVW(&htim1, TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3);
LED led1(GPIOA,GPIO_PIN_5);
UART_Handler uartCommunication(&huart2);

// フラグで DMA 再送信管理
volatile bool masterReady = false;
volatile bool slaveReady  = false;

// ==== プロトタイプ ====
void StartMasterTransfer(void);
void StartSlaveTransfer(void);
void PrintBuffer(const char* label, uint8_t* buf, size_t len);

// ==== 送受信開始関数 ====
void StartMasterTransfer(void) {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET); // CS Low
    HAL_SPI_TransmitReceive_DMA(&hspi2, masterTx, masterRx, SPI_LEN);
}

void StartSlaveTransfer(void) {
    HAL_SPI_TransmitReceive_DMA(&hspi3, slaveTx, slaveRx, SPI_LEN);
}

// ==== バッファ内容をUART出力 ====
void PrintBuffer(const char* label, uint8_t* buf, size_t len) {
    char msg[64];
    int pos = snprintf(msg, sizeof(msg), "%s:", label);
    for (size_t i = 0; i < len; i++) {
        pos += snprintf(msg + pos, sizeof(msg) - pos, " %d", buf[i]);
    }
    snprintf(msg + pos, sizeof(msg) - pos, "\r\n");
    uartCommunication.sendMessage(msg);
}

// ==== タイマ割り込み ====
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM2) {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_0);
    }
    if (htim == &htim7) {
        led_pwm.update();
        phaseUVW.update();
    }
}

// ==== SPI送受信完了コールバック ====
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi) {
    if (hspi->Instance == SPI2) { // Master
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET); // CS High
        PrintBuffer("[Master] Rx", masterRx, SPI_LEN);
        // 次回送信データを更新
        for(int i=0;i<SPI_LEN;i++) masterTx[i] = masterRx[i]+1;
        masterReady = true; // 再送信は loop で
    }
    else if (hspi->Instance == SPI3) { // Slave
        PrintBuffer("[Slave] Rx", slaveRx, SPI_LEN);
        for(int i=0;i<SPI_LEN;i++) slaveTx[i] = slaveRx[i]+1;
        slaveReady = true;
    }
}

// ==== SPIエラーコールバック ====
void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi) {
    if(hspi->Instance == SPI2) {
        uartCommunication.sendMessage("Master SPI Error\r\n");
        HAL_SPI_Abort(&hspi2);
        masterReady = true; // 再送信フラグ
    }
    if(hspi->Instance == SPI3) {
        uartCommunication.sendMessage("Slave SPI Error\r\n");
        HAL_SPI_Abort(&hspi3);
        slaveReady = true;
    }
}

// ==== setupとloop ====
void setup() {
    led1.off();
    led_pwm.start();
    phaseUVW.start();

    // 先に Slave DMA 待機
    StartSlaveTransfer();

    HAL_Delay(1);

    // Master DMA は CS Low して開始
    StartMasterTransfer();

    uartCommunication.sendMessage("UART Initialized\r\n");

    HAL_TIM_Base_Start_IT(&htim7);
}

void loop() {
    // DMA 再送信はフラグで管理
    if (slaveReady) { StartSlaveTransfer(); slaveReady=false; }
    if (masterReady){ StartMasterTransfer(); masterReady=false; }
}
