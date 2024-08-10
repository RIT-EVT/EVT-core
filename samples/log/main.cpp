/**
 * Example that logs sample statements with some sample data.
 *
 * The sample will log only the info, warning, and error statements because the
 * logger's level has been set to info.
 */

#include <cstdint>

#include <core/manager.hpp>
#include <core/utils/log.hpp>

namespace IO  = core::IO;
namespace log = core::log;
namespace DEV = core::DEV;

int main() {
    // Initialize system
    core::platform::init();

    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);

    // Print directly to the UART to show that the file has been run despite any
    // problems with the Logger
    uart.printf("Starting log test\n\r");

    // Set up the logger with a UART, logLevel, and clock
    // If timestamps aren't needed, don't set the logger's clock
    log::LOGGER.setUART(&uart);
    log::LOGGER.setLogLevel(log::Logger::LogLevel::INFO);
    DEV::RTC& rtc = DEV::getRTC();
    log::LOGGER.setClock(&rtc);

    uint8_t sampleData = 0xab;

    // Attempt to log a statement at each log level
    log::LOGGER.log(log::Logger::LogLevel::DEBUG, "Log 0");
    log::LOGGER.log(log::Logger::LogLevel::INFO, "Log 1");
    log::LOGGER.log(log::Logger::LogLevel::WARNING, "Log 2 - %x", sampleData);
    log::LOGGER.log(log::Logger::LogLevel::ERROR, "Log 3 - %d", sampleData);
}
