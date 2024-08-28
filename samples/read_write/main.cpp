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

namespace io   = core::io;
namespace dev  = core::dev;
namespace time = core::time;

int main() {
    // Initialize system
    core::platform::init();

    // Setup the GPIO input pin
    io::GPIO& inputGPIO = io::getGPIO<io::Pin::PC_3>(io::GPIO::Direction::INPUT);

    // Setup the GPIO output pin
    io::GPIO& ledGPIO = io::getGPIO<io::Pin::LED>();
    dev::LED led(ledGPIO, dev::LED::ActiveState::HIGH);

    while (1) {
        io::GPIO::State state = inputGPIO.readPin();
        led.setState(state);

        time::wait(10);
    }
}
