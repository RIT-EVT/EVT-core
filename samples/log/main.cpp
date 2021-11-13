/**
 * TODO: Explanation of this sample
 */
#include <cstdint>

#include <EVT/io/manager.hpp>
#include <EVT/utils/log.hpp>

namespace IO = EVT::core::IO;
namespace log = EVT::core::log;

int main() {
    // Initialize system
    IO::init();

    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);

    log::LOGGER.setUART(&uart);

    log::LOGGER.log("Log\n\r");
    log::LOGGER.log("Log part 2\n\r");
}
