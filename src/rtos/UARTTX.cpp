#include <EVT/rtos/UARTTX.hpp>
#include <cstdarg>
#include <cstdio>

namespace IO = EVT::core::IO;
namespace core::rtos::wrapper {

UARTTX::UARTTX(IO::UART& uart) : uart(uart) {}

TXError UARTTX::init(BytePoolBase &pool) {
    return Success;
}

void UARTTX::printf(const char* format, ...) {
    va_list args; /* Access the variable argument list */
    va_start(args, format); /* Tells the args variable to point to the format parameter first */
    char buffer[256]; /* Buffer array to hold the message */
    vsnprintf(buffer, sizeof(buffer), format, args); /* vsnprint formats the string and stores it in the buffer array */
    uart.printf(buffer); /* Send the buffer data over UART */
    va_end(args); /* Cleans va_list once the message has been sent */
}

}// namespace core::rtos::wrapper