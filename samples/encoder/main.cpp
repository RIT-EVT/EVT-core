/**
 * This is an example designed to work with the charge controller,
 * but please ask someone before flashing the charge controller!
*/

#include <EVT/dev/encoder.hpp>
#include <EVT/io/UART.hpp>
#include <EVT/io/pin.hpp>
#include <EVT/manager.hpp>
#include <EVT/utils/time.hpp>


namespace IO = EVT::core::IO;
namespace DEV = EVT::core::DEV;
namespace time = EVT::core::time;

constexpr IO::Pin A_PIN = IO::Pin::PA_8;
constexpr IO::Pin B_PIN = IO::Pin::PA_9;

constexpr uint8_t deviceCount = 1;
IO::GPIO* devices[deviceCount];

int main() {
    //Init platform
    EVT::core::platform::init();
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600, true);

    uart.printf("\n\rSTARTING ENCODER TEST\n\r");

    IO::GPIO& pinA = IO::getGPIO<A_PIN>(IO::GPIO::Direction::INPUT);
    IO::GPIO& pinB = IO::getGPIO<B_PIN>(IO::GPIO::Direction::INPUT);

    DEV::Encoder encoder(pinA, pinB, 124, 0, true);

    while(1) {
        //ENCODER MUST BE UPDATED EACH LOOP
        //That is how the rotation is read
        int8_t change = encoder.update();
        //Now that the encoder is updated we can read the position of it, which for this example will be between [-124, 124]
        uint64_t position = encoder.getPosition();
        uint8_t noChangeCounter = encoder.getNoChangeCounter();

        //PRINT VALUES (only enable one at a time)
        //uart.printf("\r Encoder Change: %d       ", change);
        //uart.printf("\rRelative Pos: %d          ", currentRelPos);
        uart.printf("\rPosition: %d         ", position);
        //uart.printf("\rNoChangeCounter: %d          ", noChangeCounter);

        //The wait simulates a loop that is doing other processing, because that will affect how often the output is read
        time::wait(300);
    }
}
