/**
 * Example of IWDG functionality. The timer is refreshed 3 times in a loop,
 * then hits an infinite loop in the code which triggers a system reset.
 */
#include <EVT/io/manager.hpp>
#include <EVT/utils/time.hpp>
#include <EVT/dev/platform/f3xx/f302x8/IWDGf302x8.hpp>

namespace IO = EVT::core::IO;
namespace DEV = EVT::core::DEV;
namespace time = EVT::core::time;

int main() {
    // Initialize system
    IO::init();
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);

    // Start watchdog
    DEV::IWDGf302x8 iwdg = DEV::IWDGf302x8(5000);

    uart.printf("Starting IWDG test...\n\r\n\r");

    // Refresh watchdog at regular intervals
    for (int i = 0; i < 3; i++) {
        time::wait(500);
        iwdg.refresh();
        uart.printf("IWDG refreshed\n\r");
    }

    // Infinite loop represents an error, board should reset after the watchdog times out
    while (1);
}