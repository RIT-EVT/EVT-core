#include <EVT/io/UART.hpp>
#include <EVT/io/pin.hpp>
#include <EVT/manager.hpp>
#include <EVT/dev/encoder.hpp>


namespace IO = EVT::core::IO;
namespace DEV = EVT::core::DEV;

constexpr IO::Pin A_PIN = IO::Pin::PA_8;
constexpr IO::Pin B_PIN = IO::Pin::PA_9;

/**
 * encoder Button: PB_4
 * encoder A: PA_8
 * encoder B: PA_9
 *
 * This is an example designed to work with the charge controller, but please ask someone before flashing the charge controller!
*/


int main() {
    EVT::core::platform::init();
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);
    uart.printf("Starting Encoder Test\n\r");

    IO::GPIO& pinA = IO::getGPIO<A_PIN>(IO::GPIO::Direction::INPUT);
    IO::GPIO& pinB = IO::getGPIO<B_PIN>(IO::GPIO::Direction::INPUT);

    DEV::Encoder encoder(pinA, pinB, 124, 0);

    while(1) {
        //ENCODER MUST BE UPDATED EACH LOOP
        //That is how the rotation is read
        int8_t change = encoder.update();
        //Now that the encoder is updated we can read the position of it, which for this example will be between [-124, 124]
        int64_t position = encoder.getPosition();
        uint8_t noChangeCounter = encoder.getNoChangeCounter();
        uart.printf("Encoder Update:\n\r");
        uart.printf("\t Encoder Change: %d \n\r", change);
        uart.printf("\t Encoder Position: %d \n\r", position);
        uart.printf("\t Encoder noChangeCounter: %d \n\r", noChangeCounter);
    }
}