#ifndef EVT_TX_UART_H
#define EVT_TX_UART_H

#include <EVT/rtos/Initializable.hpp>
#include <EVT/io/UART.hpp>

namespace IO = EVT::core::IO;
namespace core::rtos::wrapper {

class UARTTX: Initializable {
public:

    /**
     * Constructor for thread safe uart class
     *
     * @param[in] uart A UART instance
     */
    UARTTX(IO::UART&);

    TXError init(BytePoolBase &pool) override;

private:

    IO::UART& uart;
};

}// namespace core::rtos::wrapper

#endif//EVT_TX_UART_H
