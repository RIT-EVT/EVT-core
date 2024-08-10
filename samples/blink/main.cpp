/**
 * This example is the basic LED blink sample. This samples shows how the
 * device level drivers (the LED in this case) can be established by providing
 * the driver with the cooresponding IO level driver (GPIO in this case).
 *
 * The code will setup a GPIO pin as an output then setup an LED using that
 * GPIO.
 */
#include <core/dev/LED.hpp>
#include <core/io/GPIO.hpp>
#include <core/io/pin.hpp>
#include <core/manager.hpp>
#include <core/utils/time.hpp>

namespace IO = core::IO;
namespace DEV = core::DEV;
namespace time = core::time;

int main() {
    // Initialize system
    core::platform::init();

    // Setup the GPIO pin.
    // Notice that the pin used is called "LED". Each platform has a dedicated
    // LED pin, for the f3xx that is PB_13.
    IO::GPIO& ledGPIO = IO::getGPIO<IO::Pin::LED>();
    DEV::LED led(ledGPIO, DEV::LED::ActiveState::HIGH);

    while (1) {
        led.toggle();

        // Wait half a second
        time::wait(500);
    }

    return 0;
}
