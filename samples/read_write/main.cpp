/**
 * This example shows off using GPIO pins for input and output. One GPIO is
 * setup as an input and the state of that GPIO is reflected in the state of
 * the other GPIO connected to an LED.
 */
#include <EVT/io/GPIO.hpp>
#include <EVT/io/manager.hpp>
#include <EVT/io/pin.hpp>
#include <EVT/dev/LED.hpp>
#include <EVT/utils/time.hpp>

namespace IO = EVT::core::IO;
namespace DEV = EVT::core::DEV;
namespace time = EVT::core::time;

int main() {
    // Initialize system
    IO::init();

    // Setup the GPIO input pin
    IO::GPIO& inputGPIO = IO::getGPIO<IO::Pin::PC_3>(
            IO::GPIO::Direction::INPUT);

    // Setup the GPIO output pin
    IO::GPIO& ledGPIO = IO::getGPIO<IO::Pin::LED>();
    DEV::LED led(ledGPIO, DEV::LED::ActiveState::HIGH);

    while (1) {
        IO::GPIO::State state = inputGPIO.readPin();
        led.setState(state);

        time::wait(10);
    }
}
