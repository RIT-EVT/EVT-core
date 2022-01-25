#include <EVT/dev/LED.hpp>
#include <EVT/dev/button.hpp>
#include <EVT/io/UART.hpp>
#include <EVT/io/manager.hpp>

namespace DEV = EVT::core::DEV;
namespace IO = EVT::core::IO;

int main() {
    // Initialize system
    IO::init();

    // Setup UART
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);
    IO::GPIO& ledGPIO = IO::getGPIO<IO::Pin::LED>();
    IO::GPIO& buttonGPIO = IO::getGPIO<IO::Pin::PC_3>();

    DEV::LED led(ledGPIO, DEV::LED::ActiveState::HIGH);

    DEV::Button button = DEV::Button(buttonGPIO, DEV::Button::LogicLevelOnPress::HIGH);

    while (1) {

        IO::GPIO::State buttonStatus = button.debounce(DEV::Button::LogicLevelOnPress::HIGH, 100) {
            led.toggle();
            time::wait(1000);
        }

        led.toggle();

    }


    return 0;
}
