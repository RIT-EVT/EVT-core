#include <stdint.h>
#include <string.h>

#include <EVT/io/UART.hpp>

namespace EVT::core::IO {

UART::UART(Pin txPin, Pin rxPin, uint32_t baudrate, uint8_t portID) {
    this->txPin = txPin;
    this->rxPin = rxPin;
    this->baudrate = baudrate;
    this->portID = portID;
}

char* UART::gets(char* buf, size_t size) {
    char ret = '\0';
    size_t buf_index = 0;
    memset(buf, 0, size);

    while (ret != '\r' && ret != '\n' && ret != '\4') {  // '\4' is EOF
        ret = getc();

        // Check if backspace key is entered
        if (ret == '\b') {
            if (buf_index > 0 && buf_index < size) {
                buf[--buf_index] = '\0';
                putc('\b');
                putc(' ');
                putc('\b');
            }
        } else if (buf_index < (size - 1)
                && ret != '\r' && ret != '\n' && ret != '\4') {
            buf[buf_index++] = ret;
            putc(ret);
        }
    }

    return buf;
}

}  // namespace EVT::core::IO
