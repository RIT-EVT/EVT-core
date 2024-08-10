#include <core/dev/LED.hpp>
#include <core/dev/button.hpp>
#include <core/io/UART.hpp>
#include <core/manager.hpp>
#include <core/utils/time.hpp>

namespace DEV  = core::DEV;
namespace IO   = core::IO;
namespace time = core::time;

int main() {
    // Initialize system
    core::platform::init();

    // Setup UART
    IO::UART& uart         = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);
    IO::GPIO& ledGPIO      = IO::getGPIO<IO::Pin::LED>();
    IO::GPIO& buttonGPIO   = IO::getGPIO<IO::Pin::PC_3>();
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
