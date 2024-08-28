/**
 * This is a basic sample of using the UART module. The program provides a
 * basic echo functionality where the uart will write back whatever the user
 * enters.
 */
#include <core/io/UART.hpp>
#include <core/io/pin.hpp>
#include <core/manager.hpp>

namespace io = core::io;

constexpr int BAUD_RATE         = 9600;
constexpr io::Pin INTERRUPT_PIN = io::Pin::PC_3;

io::UART* uart;

void risingEdgeHandler(io::GPIO* pin, void* priv) {
    // we don't need the void* in this handler, but the registerIRQ()
    // method requires a function with these exact arguments
    io::GPIO::State pin_value = pin->readPin();

    uart->printf("Received %s edge interrupt for pin C3\n\r",
                 pin_value == io::GPIO::State::HIGH ? "rising" : "falling");
    uart->printf("Pin Value: %d\n\r", static_cast<uint32_t>(pin_value));
}

int main() {
    // Initialize system
    core::platform::init();

    // Setup UART
    uart = &io::getUART<io::Pin::UART_TX, io::Pin::UART_RX>(BAUD_RATE);

    // Set the GPIO interrupt
    io::GPIO& interruptGPIO = io::getGPIO<INTERRUPT_PIN>(io::GPIO::Direction::INPUT);
    interruptGPIO.registerIRQ(io::GPIO::TriggerEdge::RISING_FALLING, risingEdgeHandler, nullptr);

    uart->printf("\n\rWaiting for interrupts...\n\r");

    while (1) {
        continue;
    }
}
