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

    io::ADC& adc00 = io::getADC<io::Pin::PA_0, io::ADCPeriph::ONE>();
    io::ADC& adc01 = io::getADC<io::Pin::PC_3, io::ADCPeriph::ONE>();
    io::ADC& adc02 = io::getADC<io::Pin::PC_4, io::ADCPeriph::ONE>();

    io::ADC& adc10 = io::getADC<io::Pin::PA_0, io::ADCPeriph::TWO>();
    io::ADC& adc11 = io::getADC<io::Pin::PC_3, io::ADCPeriph::TWO>();
    io::ADC& adc12 = io::getADC<io::Pin::PC_4, io::ADCPeriph::TWO>();

    io::ADC& adc20 = io::getADC<io::Pin::PA_0, io::ADCPeriph::THREE>();
    io::ADC& adc21 = io::getADC<io::Pin::PC_3, io::ADCPeriph::THREE>();
    io::ADC& adc22 = io::getADC<io::Pin::PC_2, io::ADCPeriph::THREE>();

    while (1) {
        core::log::LOGGER.log(core::log::Logger::LogLevel::INFO, "--------------------");
        core::log::LOGGER.log(
            core::log::Logger::LogLevel::INFO, "ADC1 : %d mV", static_cast<uint32_t>(adc00.read() * 1000));
        core::log::LOGGER.log(
            core::log::Logger::LogLevel::INFO, "ADC1: %d%%", static_cast<uint32_t>(adc00.readPercentage() * 100));
        core::log::LOGGER.log(core::log::Logger::LogLevel::INFO, "ADC1 raw: %d\r\n", adc00.readRaw());

        core::log::LOGGER.log(
            core::log::Logger::LogLevel::INFO, "ADC1 : %d mV", static_cast<uint32_t>(adc01.read() * 1000));
        core::log::LOGGER.log(
            core::log::Logger::LogLevel::INFO, "ADC1: %d%%", static_cast<uint32_t>(adc01.readPercentage() * 100));
        core::log::LOGGER.log(core::log::Logger::LogLevel::INFO, "ADC1 raw: %d\r\n", adc01.readRaw());

        core::log::LOGGER.log(
            core::log::Logger::LogLevel::INFO, "ADC1 : %d mV", static_cast<uint32_t>(adc02.read() * 1000));
        core::log::LOGGER.log(
            core::log::Logger::LogLevel::INFO, "ADC1: %d%%", static_cast<uint32_t>(adc02.readPercentage() * 100));
        core::log::LOGGER.log(core::log::Logger::LogLevel::INFO, "ADC1 raw: %d\r\n", adc02.readRaw());
        core::log::LOGGER.log(core::log::Logger::LogLevel::INFO, "--------------------\r\n");
        core::log::LOGGER.log(core::log::Logger::LogLevel::INFO, "--------------------");
        core::log::LOGGER.log(
            core::log::Logger::LogLevel::INFO, "ADC2 : %d mV", static_cast<uint32_t>(adc10.read() * 1000));
        core::log::LOGGER.log(
            core::log::Logger::LogLevel::INFO, "ADC2: %d%%", static_cast<uint32_t>(adc10.readPercentage() * 100));
        core::log::LOGGER.log(core::log::Logger::LogLevel::INFO, "ADC2 raw: %d\r\n", adc10.readRaw());

        core::log::LOGGER.log(
            core::log::Logger::LogLevel::INFO, "ADC2 : %d mV", static_cast<uint32_t>(adc11.read() * 1000));
        core::log::LOGGER.log(
            core::log::Logger::LogLevel::INFO, "ADC2: %d%%", static_cast<uint32_t>(adc11.readPercentage() * 100));
        core::log::LOGGER.log(core::log::Logger::LogLevel::INFO, "ADC2 raw: %d\r\n", adc11.readRaw());

        core::log::LOGGER.log(
            core::log::Logger::LogLevel::INFO, "ADC2 : %d mV", static_cast<uint32_t>(adc12.read() * 1000));
        core::log::LOGGER.log(
            core::log::Logger::LogLevel::INFO, "ADC2: %d%%", static_cast<uint32_t>(adc12.readPercentage() * 100));
        core::log::LOGGER.log(core::log::Logger::LogLevel::INFO, "ADC2 raw: %d\r\n", adc12.readRaw());
        core::log::LOGGER.log(core::log::Logger::LogLevel::INFO, "--------------------\r\n");
        core::log::LOGGER.log(core::log::Logger::LogLevel::INFO, "--------------------");
        core::log::LOGGER.log(
            core::log::Logger::LogLevel::INFO, "ADC3 : %d mV", static_cast<uint32_t>(adc20.read() * 1000));
        core::log::LOGGER.log(
            core::log::Logger::LogLevel::INFO, "ADC3: %d%%", static_cast<uint32_t>(adc20.readPercentage() * 100));
        core::log::LOGGER.log(core::log::Logger::LogLevel::INFO, "ADC3 raw: %d\r\n", adc20.readRaw());

        core::log::LOGGER.log(
            core::log::Logger::LogLevel::INFO, "ADC3 : %d mV", static_cast<uint32_t>(adc21.read() * 1000));
        core::log::LOGGER.log(
            core::log::Logger::LogLevel::INFO, "ADC3: %d%%", static_cast<uint32_t>(adc21.readPercentage() * 100));
        core::log::LOGGER.log(core::log::Logger::LogLevel::INFO, "ADC3 raw: %d\r\n", adc21.readRaw());

        core::log::LOGGER.log(
            core::log::Logger::LogLevel::INFO, "ADC3 : %d mV", static_cast<uint32_t>(adc22.read() * 1000));
        core::log::LOGGER.log(
            core::log::Logger::LogLevel::INFO, "ADC3: %d%%", static_cast<uint32_t>(adc22.readPercentage() * 100));
        core::log::LOGGER.log(core::log::Logger::LogLevel::INFO, "ADC3 raw: %d\r\n", adc22.readRaw());
        core::log::LOGGER.log(core::log::Logger::LogLevel::INFO, "--------------------\r\n");
        time::wait(10000);
    }
}
