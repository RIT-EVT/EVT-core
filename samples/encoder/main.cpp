/**
 * This is an example designed to work with the charge controller,
 * but please ask someone before flashing the charge controller!
*/

#include <EVT/dev/encoder.hpp>
#include <EVT/io/UART.hpp>
#include <EVT/io/pin.hpp>
#include <EVT/manager.hpp>
#include <EVT/utils/time.hpp>
#include <EVT/utils/log.hpp>


namespace IO = EVT::core::IO;
namespace DEV = EVT::core::DEV;
namespace time = EVT::core::time;
namespace log = EVT::core::log;

constexpr IO::Pin A_PIN = IO::Pin::PA_8;
constexpr IO::Pin B_PIN = IO::Pin::PA_9;

constexpr uint8_t deviceCount = 1;
IO::GPIO* devices[deviceCount];

//Necessary static wrappers for the encoder to handle the pin interrupts
DEV::Encoder* encoderWrapper;
void ainterruptHandlerWrapper(IO::GPIO* pin) {
    encoderWrapper->aInterruptHandler(pin);
}

void binterruptHandlerWrapper(IO::GPIO* pin) {
    encoderWrapper->bInterruptHandler(pin);
}

int main() {
    //Init platform
    EVT::core::platform::init();
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600, true);

    uart.printf("\n\rSTARTING ENCODER TEST\n\r");

    //init logger
    log::LOGGER.setUART(&uart);
    log::LOGGER.setLogLevel(log::Logger::LogLevel::DEBUG);

    IO::GPIO& pinA = IO::getGPIO<A_PIN>(IO::GPIO::Direction::INPUT);
    IO::GPIO& pinB = IO::getGPIO<B_PIN>(IO::GPIO::Direction::INPUT);

    DEV::Encoder encoder(pinA, pinB, 18, 0, true);

    //setting the wrapper to the encoder
    encoderWrapper = &encoder;

    //Creating the interrupts
    pinA.registerIRQ(IO::GPIO::TriggerEdge::RISING_FALLING, ainterruptHandlerWrapper);
    pinB.registerIRQ(IO::GPIO::TriggerEdge::RISING_FALLING, binterruptHandlerWrapper);

    while (1) {
        //ENCODER MUST BE UPDATED EACH LOOP
        //That is how the position is updated
        encoder.update();
        //Now that the encoder is updated we can read the position of it, which for this example will in the range [0, 18]
        uint64_t position = encoder.getPosition();

        //PRINT VALUES (only enable one at a time)
        //uart.printf("\r Encoder Change: %d       ", change);
        uart.printf("\rPosition: %d         ", position);

        //The wait simulates a loop that is doing other processing, because that will affect how often the output is read
        time::wait(100);
    }
}
