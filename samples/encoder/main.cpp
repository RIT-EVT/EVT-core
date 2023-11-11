/**
 * This example demonstrates how to utilize the encoder object,
 * Two pins are used to read the encoder values via interrupts
 * Static wrappers are necessary for the encoder to handle the pin interrupts
*/

#include <EVT/dev/Encoder.hpp>
#include <EVT/io/UART.hpp>
#include <EVT/io/pin.hpp>
#include <EVT/manager.hpp>
#include <EVT/utils/log.hpp>
#include <EVT/utils/time.hpp>

namespace IO = EVT::core::IO;
namespace DEV = EVT::core::DEV;
namespace time = EVT::core::time;
namespace log = EVT::core::log;

constexpr IO::Pin A_PIN = IO::Pin::PA_8;
constexpr IO::Pin B_PIN = IO::Pin::PA_9;

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

    while (1) {
        //ENCODER MUST BE UPDATED EACH LOOP
        //Read the position of the encoder, which for this example will be in the range [0, 18]
        uint64_t position = encoder.getPosition();

        //PRINT VALUES (only enable one at a time)
        //uart.printf("\r Encoder Change: %d       ", change);
        uart.printf("\rPosition: %d         ", position);

        //The wait simulates a loop that is doing other processing, because that will affect how often the output is read
        time::wait(1000);
    }
}
