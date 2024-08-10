#include <core/dev/LED.hpp>
#include <core/dev/button.hpp>
#include <core/io/UART.hpp>
#include <core/manager.hpp>
#include <core/utils/time.hpp>

namespace DEV = EVT::core::DEV;
namespace IO = EVT::core::IO;
namespace time = EVT::core::time;

int main() {
    // Initialize system
    EVT::core::platform::init();

    // Setup UART
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);
    IO::GPIO& ledGPIO = IO::getGPIO<IO::Pin::LED>();
    IO::GPIO& buttonGPIO = IO::getGPIO<IO::Pin::PC_3>();

    DEV::LED led(ledGPIO, DEV::LED::ActiveState::HIGH);

    DEV::Button button = DEV::Button(buttonGPIO);

    while (true) {
        IO::GPIO::State buttonStatus = button.getState();
        uart.printf("Button Status: %d \n", buttonStatus);

        led.setState(buttonStatus);

        time::wait(100);
    }

    return 0;
}
