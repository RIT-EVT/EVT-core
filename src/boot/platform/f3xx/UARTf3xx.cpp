#include <HALf3/stm32f3xx_hal_def.h>

#include <boot/UART.hpp>
#include <cstdarg>
#include <cstdio>
#include <cstring>

namespace boot {

void UART::write(uint8_t* data, size_t size) {
    HAL_UART_Transmit(&halUART, data, size, HAL_MAX_DELAY);
}

void UART::read(uint8_t* data, size_t size) {}

void UART::setIRQHandler(void (*handler)(IOInterface& inter, void* priv), void* priv) {
    this->handler = handler;
    this->priv    = priv;
}

void UART::printf(const char* format, ...) {
    va_list args;
    va_start(args, format);

    char string[256];
    auto* data = reinterpret_cast<uint8_t*>(&string);
    if (0 < vsnprintf(string, sizeof(string), format, args)) {
        string[255] = '\0';
        write(data, strlen(string));
    }

    va_end(args);
}
} // namespace boot:UARTf3xx