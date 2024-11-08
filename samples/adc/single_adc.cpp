/**
 * This example shows off the use of the ADC. Two pins are setup for ADC
 * functionality and the values are continuously read in and printed over
 * UART.
 */
#include <core/io/ADC.hpp>
#include <core/io/UART.hpp>
#include <core/manager.hpp>
#include <core/utils/log.hpp>
#include <core/utils/time.hpp>

namespace io   = core::io;
namespace time = core::time;

int main() {
    // Initialize system
    core::platform::init();

    io::UART& uart = io::getUART<io::Pin::UART_TX, io::Pin::UART_RX>(9600);

    // Set up the logger to catch errors in ADC creation
    core::log::LOGGER.setUART(&uart);
    core::log::LOGGER.setLogLevel(core::log::Logger::LogLevel::INFO);

    uart.printf("Starting ADC test\r\n");

    time::wait(500);

    io::ADC& adc0 = io::getADC<io::Pin::PA_0, io::ADCPeriph::ONE>();

    while (1) {
        core::log::LOGGER.log(core::log::Logger::LogLevel::INFO, "--------------------\r\n");
        core::log::LOGGER.log(
            core::log::Logger::LogLevel::INFO, "ADC0 : %d mV\r\n", static_cast<uint32_t>(adc0.read() * 1000));
        core::log::LOGGER.log(
            core::log::Logger::LogLevel::INFO, "ADC0: %d%%\r\n", static_cast<uint32_t>(adc0.readPercentage() * 100));
        core::log::LOGGER.log(core::log::Logger::LogLevel::INFO, "ADC0 raw: %d\r\n\r\n", adc0.readRaw());
        core::log::LOGGER.log(core::log::Logger::LogLevel::INFO, "--------------------\r\n\r\n");
        time::wait(500);
    }
}
