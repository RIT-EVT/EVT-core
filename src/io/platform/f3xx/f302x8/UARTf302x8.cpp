#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#include <EVT/io/pin.hpp>
#include <EVT/io/platform/f3xx/f302x8/UARTf302x8.hpp>

#include <HALf3/stm32f3xx.h>

namespace EVT::core::IO {

UARTf302x8::UARTf302x8(Pin txPin, Pin rxPin, uint32_t baudrate)
    : UART(txPin, rxPin, baudrate) {

    GPIO_InitTypeDef gpioInit;

    // Determine portID
    uint8_t portID = 1;

    switch (txPin) {
        case Pin::PA_9:
        case Pin::PB_6:
        case Pin::PC_4:
            portID = 1;
            break;
        case Pin::PA_2:
        case Pin::PA_14:
        case Pin::PB_3:
            portID = 2;
            break;
        case Pin::PB_9:
        case Pin::PB_10:
        case Pin::PC_10:
            portID = 3;
            break;
        default:
            break;
    }

    switch (portID) {
        case 1:
            halUART.Instance = USART1;

            if (!(__HAL_RCC_USART1_IS_CLK_ENABLED()))
                __HAL_RCC_USART1_CLK_ENABLE();

            gpioInit.Alternate = GPIO_AF7_USART1;

            break;
        case 2:
            halUART.Instance = USART2;

            if (!(__HAL_RCC_USART2_IS_CLK_ENABLED()))
                __HAL_RCC_USART2_CLK_ENABLE();

            gpioInit.Alternate = GPIO_AF7_USART2;

            break;
        case 3:
            halUART.Instance = USART3;

            if (!(__HAL_RCC_USART3_IS_CLK_ENABLED()))
                __HAL_RCC_USART3_CLK_ENABLE();

            gpioInit.Alternate = GPIO_AF7_USART3;

            break;
        default:
            break;
    }

    Pin myPins[2] = {txPin, rxPin};

    gpioInit.Pin = static_cast<uint32_t>(1 <<
            (static_cast<uint32_t>(myPins[0]) & 0x0F));
    gpioInit.Pin |= static_cast<uint32_t>(1 <<
            (static_cast<uint32_t>(myPins[1]) & 0x0F));
    gpioInit.Mode = GPIO_MODE_AF_PP;
    gpioInit.Pull = GPIO_NOPULL;
    gpioInit.Speed = GPIO_SPEED_FREQ_HIGH;

    for (uint8_t i = 0; i < 2; i++) {
        switch ((static_cast<uint8_t>(myPins[i]) & 0xF0) >> 4) {
            case 0x0:
                __HAL_RCC_GPIOA_CLK_ENABLE();
                HAL_GPIO_Init(GPIOA, &gpioInit);
                break;
            case 0x1:
                __HAL_RCC_GPIOB_CLK_ENABLE();
                HAL_GPIO_Init(GPIOB, &gpioInit);
                break;
            case 0x2:
                __HAL_RCC_GPIOC_CLK_ENABLE();
                HAL_GPIO_Init(GPIOC, &gpioInit);
                break;
            case 0x3:
                __HAL_RCC_GPIOD_CLK_ENABLE();
                HAL_GPIO_Init(GPIOD, &gpioInit);
                break;
            default:
                break;
        }
    }

    halUART.Init.BaudRate     = baudrate;
    halUART.Init.WordLength   = UART_WORDLENGTH_8B;
    halUART.Init.StopBits     = UART_STOPBITS_1;
    halUART.Init.Parity       = UART_PARITY_NONE;
    halUART.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    halUART.Init.Mode         = UART_MODE_TX_RX;
    halUART.Init.OverSampling = UART_OVERSAMPLING_16;

    HAL_UART_Init(&halUART);
}

void UARTf302x8::setBaudrate(uint32_t baudrate) {
    this->halUART.Init.BaudRate = baudrate;
    this->baudrate = baudrate;
}

void UARTf302x8::setFormat(WordLength wordLength, Parity parity,
        NumStopBits numStopBits) {
    halUART.Init.WordLength = static_cast<uint32_t>(wordLength);
    halUART.Init.Parity = static_cast<uint32_t>(parity);
    halUART.Init.Parity = static_cast<uint32_t>(numStopBits);
}

void UARTf302x8::sendBreak() {
    HAL_LIN_SendBreak(&halUART);
}

bool UARTf302x8::isReadable() {
    return halUART.pRxBuffPtr != NULL;
}

bool UARTf302x8::isWritable() {
    return halUART.pTxBuffPtr == NULL;
}

void UARTf302x8::putc(char c) {
    uint8_t* data = reinterpret_cast<uint8_t*>(&c);
    HAL_UART_Transmit(&halUART, data, 1 , DEFAULT_TIMEOUT);
}

void UARTf302x8::puts(const char* s) {
    char buf[100];
    strncpy(buf, s, 100);
    uint8_t* data = reinterpret_cast<uint8_t*>(buf);
    HAL_UART_Transmit(&halUART, data, strlen(buf), DEFAULT_TIMEOUT);
}

char UARTf302x8::getc() {
    uint8_t c;
    HAL_UART_Receive(&halUART, &c, 1, DEFAULT_TIMEOUT);
    return static_cast<char>(c);
}

void UARTf302x8::printf(const char *format, ...) {
    va_list args;
    va_start(args, format);

    char string[200];
    uint8_t* data = reinterpret_cast<uint8_t*>(&string);
    if (0 < vsprintf(string, format, args)) {
        HAL_UART_Transmit(&halUART, data,
                strlen(string), DEFAULT_TIMEOUT);
    }

    va_end(args);
}

void UARTf302x8::write(uint8_t byte) {
    putc(static_cast<uint8_t>(byte));
}

uint8_t UARTf302x8::read() {
    return static_cast<uint8_t>(getc());
}

void UARTf302x8::writeBytes(uint8_t* bytes, size_t size) {
    HAL_UART_Transmit(&halUART, bytes, size, DEFAULT_TIMEOUT);
}

void UARTf302x8::readBytes(uint8_t* bytes, size_t size) {
    HAL_UART_Receive(&halUART, bytes, size, DEFAULT_TIMEOUT);
}

}  // namespace EVT::core::IO
