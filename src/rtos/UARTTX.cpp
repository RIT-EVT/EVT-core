#include <EVT/rtos/UARTTX.hpp>

namespace IO = EVT::core::IO;
namespace core::rtos::wrapper {

UARTTX::UARTTX(IO::UART& uart) : uart(uart) {}

TXError UARTTX::init(BytePoolBase &pool) {
    return Success;
}

}// namespace core::rtos::wrapper