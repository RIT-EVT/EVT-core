/**
 * This example prints out temperature reading from a thermistor
 */
#include <core/dev/Thermistor.hpp>
#include <core/io/ADC.hpp>
#include <core/io/UART.hpp>
#include <core/manager.hpp>
#include <core/utils/time.hpp>

namespace io   = core::io;
namespace dev  = core::dev;
namespace time = core::time;

/**
 * Note, this conversion function is just to show off how a conversion
 * function will look like. In actuality, this will be determined through
 * testing/reading the thermistor datasheet.
 */
uint32_t convert(uint32_t voltage) { return voltage / 2; }

int main() {
    core::platform::init();

    // Setup IO
    io::UART& uart = io::getUART<io::Pin::UART_TX, io::Pin::UART_RX>(9600);

    time::wait(500);

    io::ADC& adc = io::getADC<io::Pin::PA_0>();

    time::wait(500);

    // Setup the thermistor device
    dev::Thermistor thermistor(adc, convert);

    uart.printf("Starting thermistor test\r\n");

    while (1) {
        uart.printf("Temperature: %dmC\r\n", static_cast<int>(thermistor.getTempCelcius()));
        uart.printf("Voltage: %dV\r\n", static_cast<int>(thermistor.getRawADC()));
        time::wait(100);
    }
}
