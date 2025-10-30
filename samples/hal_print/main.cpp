#include "HALf3/stm32f3xx_hal.h"
#include <core/platform/f3xx/stm32f3xx.hpp>

#define PA_2  0x02
#define PA_3  0x03
#define PA_11 0x0B
#define PA_12 0x0C

static constexpr uint32_t DEFAULT_BAUD = 500000;

UART_HandleTypeDef halUART;
CAN_HandleTypeDef halCAN;

void init_uart() {
    GPIO_InitTypeDef gpioInit;
    halUART.Instance = USART2;

    if (!(__HAL_RCC_USART2_IS_CLK_ENABLED()))
        __HAL_RCC_USART2_CLK_ENABLE();

    gpioInit.Pin = static_cast<uint32_t>(1 << (PA_2 & 0x0F)) | static_cast<uint32_t>(1 << (PA_3 & 0x0F));

    gpioInit.Mode = GPIO_MODE_AF_PP;

    gpioInit.Pull = GPIO_NOPULL;

    gpioInit.Speed = GPIO_SPEED_FREQ_HIGH;

    gpioInit.Alternate = GPIO_AF7_USART2;

    __HAL_RCC_GPIOA_CLK_ENABLE();
    HAL_GPIO_Init(GPIOA, &gpioInit);

    halUART.Init.BaudRate     = 115200;
    halUART.Init.WordLength   = UART_WORDLENGTH_8B;
    halUART.Init.StopBits     = UART_STOPBITS_1;
    halUART.Init.Parity       = UART_PARITY_NONE;
    halUART.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    halUART.Init.Mode         = UART_MODE_TX_RX;
    halUART.Init.OverSampling = UART_OVERSAMPLING_16;

    HAL_UART_Init(&halUART);
}

void init_can() {
    GPIO_InitTypeDef gpioInit;

    halCAN.Instance                  = CAN1;
    halCAN.Init.Prescaler            = (HAL_RCC_GetHCLKFreq() / DEFAULT_BAUD / 16);
    halCAN.Init.Mode                 = CAN_MODE_NORMAL;
    halCAN.Init.SyncJumpWidth        = CAN_SJW_1TQ;
    halCAN.Init.TimeSeg1             = CAN_BS1_13TQ;
    halCAN.Init.TimeSeg2             = CAN_BS2_2TQ;
    halCAN.Init.TimeTriggeredMode    = DISABLE;
    halCAN.Init.AutoBusOff           = DISABLE;
    halCAN.Init.AutoWakeUp           = DISABLE;
    halCAN.Init.AutoRetransmission   = DISABLE;
    halCAN.Init.ReceiveFifoLocked    = DISABLE;
    halCAN.Init.TransmitFifoPriority = DISABLE;

    __HAL_RCC_CAN1_CLK_ENABLE();
    HAL_CAN_Init(&halCAN);

    // Intialize interrupts
    HAL_CAN_ActivateNotification(&halCAN, CAN_IT_RX_FIFO0_MSG_PENDING);
    HAL_NVIC_SetPriority(CAN_RX0_IRQn, core::platform::CAN_INTERRUPT_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(CAN_RX0_IRQn);

    // Configuring GPIO
    gpioInit.Pin = static_cast<uint32_t>(1 << (PA_12 & 0x0F)) | static_cast<uint32_t>(1 << (PA_11 & 0x0F));

    gpioInit.Mode      = GPIO_MODE_AF_OD;
    gpioInit.Pull      = GPIO_PULLUP;
    gpioInit.Speed     = GPIO_SPEED_FREQ_HIGH;
    gpioInit.Alternate = GPIO_AF9_CAN;

    __HAL_RCC_GPIOA_CLK_ENABLE();
    HAL_GPIO_Init(GPIOA, &gpioInit);

    // By default - filter that accepts all incoming messages
    CAN_FilterTypeDef defaultFilter;
    defaultFilter.FilterIdHigh         = 0;
    defaultFilter.FilterIdLow          = 0;
    defaultFilter.FilterMaskIdHigh     = 0x0000;
    defaultFilter.FilterMaskIdLow      = 0xFFFF;
    defaultFilter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    defaultFilter.FilterBank           = 0;
    defaultFilter.FilterMode           = CAN_FILTERMODE_IDMASK;
    defaultFilter.FilterScale          = CAN_FILTERSCALE_16BIT;
    defaultFilter.FilterActivation     = ENABLE;

    HAL_CAN_ConfigFilter(&halCAN, &defaultFilter);
    HAL_CAN_Start(&halCAN);
}

int main() {
    // Initialize system
    core::platform::stm32f3xx_init();
    init_uart();
    init_can();

    uint8_t data[] = "Hello World\n\r";

    HAL_UART_Transmit(&halUART, data, sizeof(data), 100);

    while (true) {}

    return 0;
}