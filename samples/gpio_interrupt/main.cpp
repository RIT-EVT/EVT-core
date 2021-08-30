/**
 * This is a basic sample of using the UART module. The program provides a
 * basic echo functionality where the uart will write back whatever the user
 * enters.
 */
#include <EVT/io/manager.hpp>
#include <EVT/io/pin.hpp>
#include <EVT/io/UART.hpp>

namespace IO = EVT::core::IO;

constexpr int BAUD_RATE = 115200;
constexpr IO::Pin INTERRUPT_PIN = IO::Pin::PC_8;

void rising_edge_handler() {
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(BAUD_RATE);
    uart.printf("Received rising edge interrupt for pin C8");
}

int main() {
    // Initialize system
    IO::init();

    // Setup UART
    IO::UART &uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(BAUD_RATE);

    // Set the GPIO interrupt
    IO::GPIO& inputGPIO = IO::getGPIO<INTERRUPT_PIN>(
            IO::GPIO::Direction::INPUT);
    inputGPIO.registerIrq(IO::GPIO::TriggerEdge::RISING, rising_edge_handler);

    uart.printf("Waiting for interrupts...");

    while (1) {
        continue;
    }
}
