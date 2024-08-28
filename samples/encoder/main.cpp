/**
 * This example demonstrates how to utilize the encoder object,
 * Two pins are used to read the encoder values via interrupts
 * Static wrappers are necessary for the encoder to handle the pin interrupts
 */

#include <core/dev/Encoder.hpp>
#include <core/io/UART.hpp>
#include <core/io/pin.hpp>
#include <core/manager.hpp>
#include <core/utils/log.hpp>
#include <core/utils/time.hpp>

namespace io   = core::io;
namespace dev  = core::dev;
namespace time = core::time;
namespace log  = core::log;

constexpr io::Pin A_PIN = io::Pin::PA_8;
constexpr io::Pin B_PIN = io::Pin::PA_9;

int main() {
    // Init platform
    core::platform::init();
    io::UART& uart = io::getUART<io::Pin::UART_TX, io::Pin::UART_RX>(9600, true);

    uart.printf("\n\rSTARTING ENCODER TEST\n\r");

    // init logger
    log::LOGGER.setUART(&uart);
    log::LOGGER.setLogLevel(log::Logger::LogLevel::DEBUG);

    io::GPIO& pinA = io::getGPIO<A_PIN>(io::GPIO::Direction::INPUT);
    io::GPIO& pinB = io::getGPIO<B_PIN>(io::GPIO::Direction::INPUT);

    dev::Encoder encoder(pinA, pinB, 18, 0, true);

    while (1) {
        // ENCODER MUST BE UPDATED EACH LOOP
        // Read the position of the encoder, which for this example will be in the range [0, 18]
        uint64_t position = encoder.getPosition();

        // PRINT VALUES (only enable one at a time)
        // uart.printf("\r Encoder Change: %d       ", change);
        uart.printf("\rPosition: %d         ", position);

        // The wait simulates a loop that is doing other processing, because that will affect how often the output is
        // read
        time::wait(1000);
    }
}
