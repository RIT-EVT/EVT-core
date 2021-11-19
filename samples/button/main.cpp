#include <EVT/dev/button.hpp>
#include <EVT/dev/LED.hpp>
#include <EVT/io/manager.hpp>

namespace DEV = EVT::core::DEV;
namespace IO = EVT::core::IO;

int main() {
    // Initialize system
    IO::init();

    
    IO::GPIO& ledGPIO = IO::getGPIO<IO::Pin::LED>();
    IO::GPIO& buttonGPIO = IO::getGPIO<IO::Pin::PC_3>();

    DEV::LED led(ledGPIO, DEV::LED::ActiveState::HIGH);

    DEV::Button button = DEV::Button(buttonGPIO, DEV::Button::ActiveState::HIGH);

    while (true) {
        IO::GPIO::State buttonStatus = button.readButton();

        led.setState(buttonStatus);
    }


    return 0;
}