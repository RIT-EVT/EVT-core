#ifndef EVT_TX_UART_H
#define EVT_TX_UART_H

#include <EVT/rtos/Initializable.hpp>
#include <EVT/io/UART.hpp>

namespace IO = EVT::core::IO;
namespace core::rtos::wrapper {

class UARTTX: Initializable {
    UARTTX(IO::UART&);
};

}// namespace core::rtos::wrapper

#endif//EVT_TX_UART_H
