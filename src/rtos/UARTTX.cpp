#include <EVT/rtos/UARTTX.hpp>
#include <cstdarg>

namespace IO = EVT::core::IO;
namespace core::rtos::wrapper {

UARTTX::UARTTX(IO::UART& uart) : uart(uart) {}

TXError UARTTX::init(BytePoolBase &pool) {
    return Success;
}

void UARTTX::printf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    uart.printf(format, args);
    va_end(args);
}

}// namespace core::rtos::wrapper