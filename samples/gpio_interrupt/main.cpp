/**
 * This is a basic sample of using the UART module. The program provides a
 * basic echo functionality where the uart will write back whatever the user
 * enters.
 */
#include <EVT/io/UART.hpp>
#include <EVT/io/manager.hpp>
#include <EVT/io/pin.hpp>

namespace IO = EVT::core::IO;

constexpr int BAUD_RATE = 9600;
constexpr IO::Pin INTERRUPT_PIN = IO::Pin::PC_3;

IO::UART* uart;

void risingEdgeHandler(IO::GPIO* pin) {
    IO::GPIO::State pin_value = pin->readPin();

    uart->printf("Received %s edge interrupt for pin C3\n\r",
                 pin_value == IO::GPIO::State::HIGH ? "rising" : "falling");
    uart->printf("Pin Value: %d\n\r", static_cast<uint32_t>(pin_value));
}

int main() {
    // Initialize system
    IO::init();

    // Setup UART
    uart = &IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(BAUD_RATE);

    // Set the GPIO interrupt
    IO::GPIO& interruptGPIO = IO::getGPIO<INTERRUPT_PIN>(
        IO::GPIO::Direction::INPUT);
    interruptGPIO.registerIRQ(IO::GPIO::TriggerEdge::RISING_FALLING,
                              risingEdgeHandler);

    uart->printf("\n\rWaiting for interrupts...\n\r");

    while (1) {
        continue;
    }
}
