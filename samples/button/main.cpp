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
    io::UART& uart       = io::getUART<io::Pin::UART_TX, io::Pin::UART_RX>(9600);
    io::GPIO& ledGPIO    = io::getGPIO<io::Pin::LED>();
    io::GPIO& buttonGPIO = io::getGPIO<io::Pin::PC_3>();

    dev::LED led(ledGPIO, dev::LED::ActiveState::HIGH);

    dev::Button button = dev::Button(buttonGPIO);

    while (true) {
        io::GPIO::State buttonStatus = button.getState();
        uart.printf("Button Status: %d \n", buttonStatus);

        led.setState(buttonStatus);

        time::wait(100);
    }

    return 0;
}
