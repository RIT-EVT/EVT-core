/**
 * TODO: Explanation of this sample
 */
#include <cstdint>

#include <EVT/io/manager.hpp>
#include <EVT/utils/log.hpp>
#include <EVT/dev/platform/f3xx/f302x8/RTC302x8.hpp>

namespace IO = EVT::core::IO;
namespace log = EVT::core::log;
namespace DEV = EVT::core::DEV;
namespace time = EVT::core::time;

int main() {
    // Initialize system
    IO::init();

    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);

    uart.printf("Starting log test\n\r");

    log::LOGGER.setUART(&uart);
    log::LOGGER.setLogLevel(log::Logger::LogLevel::WARNING);
    auto* rtc = new DEV::RTCf302x8();
    log::LOGGER.setClock(rtc);

    log::LOGGER.log(log::Logger::LogLevel::DEBUG,"Log 1\n\r");
    log::LOGGER.log(log::Logger::LogLevel::INFO,"Log 2\n\r");
    log::LOGGER.log(log::Logger::LogLevel::WARNING,"Log 3\n\r");
    log::LOGGER.log(log::Logger::LogLevel::ERROR,"Log 4\n\r");
}
