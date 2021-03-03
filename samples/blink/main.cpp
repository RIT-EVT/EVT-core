/**
 * This example is the hello world application of the EVT common software.
 * The application creates a UART connection which then repeatedly prints out
 * "hello world" at 9600 baud.
 */
#include <EVT/io/GPIO.hpp>
#include <EVT/io/platform/f3xx/GPIOf3.hpp>
#include <EVT/dev/LED.hpp>
#include <EVT/io/pin.hpp>
#include <EVT/utils/time.hpp>

namespace IO = EVT::core::IO;
namespace DEV = EVT::core::DEV;
namespace time = EVT::core::time;

int main()
{
    // Setup the GPIO pin
    IO::GPIOf3 ledGPIO(IO::Pin::PB_13, IO::GPIO::Direction::OUTPUT); 
    DEV::LED led(ledGPIO, DEV::LED::ActiveState::HIGH);
    int i = 0;

    while(1) {
        led.toggle();

        // Wait half a second
        i = 0;
        while(i < 100000) {
            i++;
        }
    }
    
    return 0;
}
