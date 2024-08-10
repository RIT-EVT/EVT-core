/**
 * This example shows off using GPIO pins for input and output. One GPIO is
 * setup as an input and the state of that GPIO is reflected in the state of
 * the other GPIO connected to an LED.
 */
#include <core/dev/LED.hpp>
#include <core/io/GPIO.hpp>
#include <core/io/pin.hpp>
#include <core/manager.hpp>
#include <core/utils/time.hpp>

namespace IO   = core::IO;
namespace DEV  = core::DEV;
namespace time = core::time;

int main() {
    // Initialize system
    core::platform::init();

    // Setup the GPIO input pin
    IO::GPIO& inputGPIO = IO::getGPIO<IO::Pin::PC_3>(IO::GPIO::Direction::INPUT);

    // Setup the GPIO output pin
    IO::GPIO& ledGPIO = IO::getGPIO<IO::Pin::LED>();
    DEV::LED led(ledGPIO, DEV::LED::ActiveState::HIGH);

    while (1) {
        IO::GPIO::State state = inputGPIO.readPin();
        led.setState(state);

        time::wait(10);
    }
}
