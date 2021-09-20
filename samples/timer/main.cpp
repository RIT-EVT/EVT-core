/**
 * This sample will demo the basic functionality for the timer driver
 */
#include <EVT/io/manager.hpp>
#include <EVT/io/pin.hpp>
#include <EVT/io/UART.hpp>
#include <EVT/dev/platform/f3xx/f302x8/Timerf302x8.hpp>

namespace IO = EVT::core::IO;
namespace DEV = EVT::core::DEV;

int main() {
    // Initialize system
    IO::init();

    // Setup UART
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);

    // Setup the Timer
    auto timer = DEV::Timerf302x8();

    // String to store user input
    char buf[100];

    while (1) {
        // Read user input
        uart.printf("Enter message: ");
        uart.gets(buf, 100);
        uart.printf("\n\recho: %s\n\r", buf);
    }
}
