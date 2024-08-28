#include <core/dev/LED.hpp>
#include <core/dev/button.hpp>
#include <core/io/UART.hpp>
#include <core/manager.hpp>
#include <core/utils/time.hpp>

namespace dev  = core::dev;
namespace io   = core::io;
namespace time = core::time;

int main() {
    // Initialize system
    core::platform::init();

    // Setup UART
    io::UART& uart         = io::getUART<io::Pin::UART_TX, io::Pin::UART_RX>(9600);
    io::GPIO& ledGPIO      = io::getGPIO<io::Pin::LED>();
    io::GPIO& buttonGPIO   = io::getGPIO<io::Pin::PC_3>();
    uint32_t debounceStart = time::millis();

    dev::LED led(ledGPIO, dev::LED::ActiveState::HIGH);

    dev::Button button = dev::Button(buttonGPIO);

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
