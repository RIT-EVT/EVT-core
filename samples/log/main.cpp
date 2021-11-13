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

int main() {
    // Initialize system
    IO::init();

    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);

    uart.printf("Starting log test\n\r");

    log::LOGGER.setUART(&uart);
    log::LOGGER.setLogLevel(log::Logger::LogLevel::DEBUG);
    auto* rtc = new DEV::RTCf302x8();
    log::LOGGER.setClock(rtc);

    uint8_t testData = 0xab;

    log::LOGGER.log(log::Logger::LogLevel::DEBUG, "Log 1");
    log::LOGGER.log(log::Logger::LogLevel::INFO, "Log 2 - %c", testData);
    log::LOGGER.log(log::Logger::LogLevel::WARNING, "Log 3 - %x", testData);
    log::LOGGER.log(log::Logger::LogLevel::ERROR, "Log 4 - %d", testData);
}
