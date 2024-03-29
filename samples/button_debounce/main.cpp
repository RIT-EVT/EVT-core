#include <EVT/dev/LED.hpp>
#include <EVT/dev/button.hpp>
#include <EVT/io/UART.hpp>
#include <EVT/manager.hpp>
#include <EVT/utils/time.hpp>

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
    uint32_t debounceStart = time::millis();

    DEV::LED led(ledGPIO, DEV::LED::ActiveState::HIGH);

    DEV::Button button = DEV::Button(buttonGPIO);

    uart.printf("Debounce Start: %d\n", debounceStart);

    while (1) {
        uart.printf("Tick: %d\n", time::millis());
        if (button.debounce(300)) {
            uart.printf("Button Pressed\n");
            led.toggle();
        }

        time::wait(1000);
    }

    return 0;
}
