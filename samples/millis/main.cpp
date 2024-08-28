/**
 * This example calls the millis function and prints out the result. Each
 * result should be ~500ms off from each other
 */
#include <core/dev/LED.hpp>
#include <core/io/GPIO.hpp>
#include <core/io/UART.hpp>
#include <core/io/pin.hpp>
#include <core/manager.hpp>
#include <core/utils/time.hpp>

namespace io   = core::io;
namespace dev  = core::dev;
namespace time = core::time;

int main() {
    // Initialize system
    core::platform::init();

    // Setup the GPIO pin.
    // Notice that the pin used is called "LED". Each platform has a dedicated
    // LED pin, for the f3xx that is PB_13.
    io::UART& uart = io::getUART<io::Pin::UART_TX, io::Pin::UART_RX>(9600);

    time::wait(500);

    while (1) {
        uart.printf("millis() -> %d\r\n", time::millis());
        // Wait half a second
        time::wait(500);
    }

    return 0;
}
