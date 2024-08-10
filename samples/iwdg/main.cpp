/**
 * Example of IWDG functionality. The timer is refreshed 3 times in a loop,
 * then hits an infinite loop in the code which triggers a system reset.
 */
#include <core/manager.hpp>
#include <core/utils/time.hpp>

namespace IO = core::IO;
namespace DEV = core::DEV;
namespace time = core::time;

int main() {
    // Initialize system
    core::platform::init();
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);

    // Start watchdog
    DEV::IWDG& iwdg = DEV::getIWDG(5000);
    iwdg.init();

    uart.printf("Starting IWDG test...\n\r\n\r");

    // Refresh watchdog at regular intervals
    for (int i = 0; i < 3; i++) {
        time::wait(500);
        iwdg.refresh();
        uart.printf("IWDG refreshed\n\r");
    }

    // Infinite loop represents an error, board should reset after the watchdog times out
    while (1)
        ;
}