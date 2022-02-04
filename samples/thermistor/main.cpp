/**
 * This example prints out the time as read in from the RTC
 * every second.
 */
#include <EVT/dev/Thermistor.hpp>
#include <EVT/io/ADC.hpp>
#include <EVT/io/UART.hpp>
#include <EVT/io/manager.hpp>
#include <EVT/utils/time.hpp>

namespace IO = EVT::core::IO;
namespace DEV = EVT::core::DEV;
namespace time = EVT::core::time;

/**
 * Note, this conversion function is just to show off how a conversion
 * function will loook like. In actuality, this will be determined through
 * testing/reading the thermistor datasheet.
 */
uint32_t convert(uint32_t voltage) {
    return voltage / 2;
}

int main() {
    IO::init();

    // Setup IO
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);

    time::wait(500);

    IO::ADC& adc = IO::getADC<IO::Pin::PA_0>();

    // TODO: Remove this once the ADC works on a single channel
    IO::ADC& adc0 = IO::getADC<IO::Pin::PA_1>();
    uart.printf("Read in ADC: %d\r\n", adc0.readRaw());

    time::wait(500);

    // Setup the thermistor device
    DEV::Thermistor thermistor(adc, convert);

    uart.printf("Starting thermistor test\r\n");

    while (1) {
        uart.printf("Temperature: %dmC\r\n",
                    static_cast<int>(thermistor.getTempCelcius()));
        time::wait(100);
    }
}
