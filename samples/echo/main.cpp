/**
 * This is a basic sample of using the UART module. The program provides a
 * basic echo functionality where the uart will write back whatever the user
 * enters.
 */
#include <EVT/io/manager.hpp>
#include <EVT/io/pin.hpp>
#include <EVT/io/UART.hpp>

namespace IO = EVT::core::IO;

int main() {
    // Initialize system
    IO::init();

    // Setup UART
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);

    // String to store user input
    char buf[100];

    while (1) {
        // Read user input
        uart.printf("Enter message: ");
        uart.gets(buf, 100);
        uart.printf("\n\recho: %s\n\r", buf);
    }
}
