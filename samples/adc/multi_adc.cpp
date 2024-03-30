/**
 * This example shows off the use of the ADC. Two pins are setup for ADC
 * functionality and the values are continuously read in and printed over
 * UART.
 */
#include <EVT/io/ADC.hpp>
#include <EVT/io/UART.hpp>
#include <EVT/manager.hpp>
#include <EVT/utils/time.hpp>

namespace IO = EVT::core::IO;
namespace time = EVT::core::time;

int main() {
    // Initialize system
    EVT::core::platform::init();

    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);

    uart.printf("Starting ADC test\r\n");

    time::wait(500);

    IO::ADC& adc0 = IO::getADC<IO::Pin::PA_0>();
    IO::ADC& adc1 = IO::getADC<IO::Pin::PA_1>();

    while (1) {
        uart.printf("--------------------\r\n");
        uart.printf("ADC0 : %d mV\r\n", static_cast<uint32_t>(adc0.read() * 1000));
        uart.printf("ADC0: %d%%\r\n", static_cast<uint32_t>(adc0.readPercentage() * 100));
        uart.printf("ADC0 raw: %d\r\n\r\n", adc0.readRaw());

        uart.printf("ADC1 : %d mV\r\n", static_cast<uint32_t>(adc1.read() * 1000));
        uart.printf("ADC1: %d%%\r\n", static_cast<uint32_t>(adc1.readPercentage() * 100));
        uart.printf("ADC1 raw: %d\r\n", adc1.readRaw());
        uart.printf("--------------------\r\n\r\n");
        time::wait(500);
    }
}
