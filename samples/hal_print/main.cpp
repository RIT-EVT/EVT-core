#include "HALf4/stm32f4xx_hal.h"
#include <core/platform/f4xx/stm32f4xx.hpp>

#define PA_2 0x02
#define PA_3 0x03

UART_HandleTypeDef halUART;

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

    halUART.Init.BaudRate     = 9600;
    halUART.Init.WordLength   = UART_WORDLENGTH_8B;
    halUART.Init.StopBits     = UART_STOPBITS_1;
    halUART.Init.Parity       = UART_PARITY_NONE;
    halUART.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    halUART.Init.Mode         = UART_MODE_TX_RX;
    halUART.Init.OverSampling = UART_OVERSAMPLING_16;

    HAL_UART_Init(&halUART);
}

int main() {
    // Initialize system
    core::platform::stm32f4xx_init();
    init_uart();

    uint8_t data[] = "Hello World\n";

    HAL_UART_Transmit(&halUART, data, sizeof(data), 100);

    while (true) {}

    return 0;
}