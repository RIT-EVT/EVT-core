/**
 * This example prints out the time as read in from the RTC
 * every second.
 */
#include <EVT/io/UART.hpp>
#include <EVT/io/ADC.hpp>
#include <EVT/io/manager.hpp>
#include <EVT/utils/time.hpp>
#include <EVT/dev/Thermistor.hpp>

namespace IO = EVT::core::IO;
namespace DEV = EVT::core::DEV;
namespace time = EVT::core::time;

int main() {
    IO::init();

    // Setup IO
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);
    IO::ADC& adc = IO::getADC<IO::Pin::PA_0>();

    // Setup the thermistor device
    DEV::Thermistor thermistor(adc, [](float voltage){
            return voltage / 2;
    });

    uart.printf("Starting thermistor test\r\n");


    time::wait(500);

    while (1) {
        uart.printf("Temperature: %dmC",
            static_cast<int>(thermistor.getTempCelcius() / 1000));
    }
}
