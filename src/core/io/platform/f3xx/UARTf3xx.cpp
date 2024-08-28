#include <core/io/platform/f3xx/UARTf3xx.hpp>

#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <cstring>

#include <HALf3/stm32f3xx.h>

#include <core/io/pin.hpp>
#include <core/io/platform/f3xx/GPIOf3xx.hpp>
#include <core/utils/log.hpp>

namespace log = core::log;

namespace core::io {

UARTf3xx::UARTf3xx(Pin txPin, Pin rxPin, uint32_t baudrate, bool isSwapped) : UART(txPin, rxPin, baudrate), halUART{} {

    GPIO_InitTypeDef gpioInit;
    Pin myPins[]      = {txPin, rxPin};
    uint8_t numOfPins = 2;
    uint8_t alt_id;

    // Determine portID
    uint8_t portID;

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
        portID = -1;
        log::LOGGER.log(log::Logger::LogLevel::ERROR, "Invalid TX Pin");
        break;
    }

    switch (portID) {
    case 1:
        halUART.Instance = USART1;

        if (!__HAL_RCC_USART1_IS_CLK_ENABLED())
            __HAL_RCC_USART1_CLK_ENABLE();

        alt_id = GPIO_AF7_USART1;

        break;
    case 2:
        halUART.Instance = USART2;

        if (!__HAL_RCC_USART2_IS_CLK_ENABLED())
            __HAL_RCC_USART2_CLK_ENABLE();

        alt_id = GPIO_AF7_USART2;

        break;
    case 3:
        halUART.Instance = USART3;

        if (!__HAL_RCC_USART3_IS_CLK_ENABLED())
            __HAL_RCC_USART3_CLK_ENABLE();

        alt_id = GPIO_AF7_USART3;

        break;
    default:
        alt_id = -1;
        log::LOGGER.log(log::Logger::LogLevel::ERROR, "Invalid Port ID");
        break;
    }

    GPIOf3xx::gpioStateInit(&gpioInit, myPins, numOfPins, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH, alt_id);

    halUART.Init.BaudRate     = baudrate;
    halUART.Init.WordLength   = UART_WORDLENGTH_8B;
    halUART.Init.StopBits     = UART_STOPBITS_1;
    halUART.Init.Parity       = UART_PARITY_NONE;
    halUART.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    halUART.Init.Mode         = UART_MODE_TX_RX;
    halUART.Init.OverSampling = UART_OVERSAMPLING_16;

    if (isSwapped) {
        halUART.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_SWAP_INIT;
        halUART.AdvancedInit.Swap           = UART_ADVFEATURE_SWAP_ENABLE;
    }

    HAL_UART_Init(&halUART);
}

void UARTf3xx::setBaudrate(uint32_t baudrate) {
    this->halUART.Init.BaudRate = baudrate;
    this->baudrate              = baudrate;
}

void UARTf3xx::setFormat(WordLength wordLength, Parity parity, NumStopBits numStopBits) {
    halUART.Init.WordLength = static_cast<uint32_t>(wordLength);
    halUART.Init.Parity     = static_cast<uint32_t>(parity);
    halUART.Init.Parity     = static_cast<uint32_t>(numStopBits);
}

void UARTf3xx::sendBreak() { HAL_LIN_SendBreak(&halUART); }

bool UARTf3xx::isReadable() { return halUART.pRxBuffPtr != nullptr; }

bool UARTf3xx::isWritable() { return halUART.pTxBuffPtr == nullptr; }

void UARTf3xx::putc(char c) {
    auto* data = reinterpret_cast<uint8_t*>(&c);
    HAL_UART_Transmit(&halUART, data, 1, EVT_UART_TIMEOUT);
}

void UARTf3xx::puts(const char* s) {
    char buf[100];
    strncpy(buf, s, 100);
    auto* data = reinterpret_cast<uint8_t*>(buf);
    HAL_UART_Transmit(&halUART, data, strlen(buf), EVT_UART_TIMEOUT);
}

char UARTf3xx::getc() {
    uint8_t c;
    while (HAL_UART_Receive(&halUART, &c, 1, EVT_UART_TIMEOUT) == HAL_TIMEOUT) {}
    return static_cast<char>(c);
}

void UARTf3xx::printf(const char* format, ...) {
    va_list args;
    va_start(args, format);

    char string[200];
    auto* data = reinterpret_cast<uint8_t*>(&string);
    if (0 < vsprintf(string, format, args)) {
        HAL_UART_Transmit(&halUART, data, strlen(string), EVT_UART_TIMEOUT);
    }

    va_end(args);
}

void UARTf3xx::write(uint8_t byte) { putc(static_cast<uint8_t>(byte)); }

uint8_t UARTf3xx::read() { return static_cast<uint8_t>(getc()); }

void UARTf3xx::writeBytes(uint8_t* bytes, size_t size) { HAL_UART_Transmit(&halUART, bytes, size, EVT_UART_TIMEOUT); }

void UARTf3xx::readBytes(uint8_t* bytes, size_t size) { HAL_UART_Receive(&halUART, bytes, size, EVT_UART_TIMEOUT); }

} // namespace core::io
