/**
 * Example that handles I2C communication between an Arduino and a
 * EVT-Core device.
 *
 * The sample will read two bytes from the Arduino which should contain
 * the bytes "o" and "k".
 */
#include <stdint.h>

#include <EVT/io/manager.hpp>
#include <EVT/utils/time.hpp>
#include <EVT/io/I2C.hpp>
#include <EVT/io/UART.hpp>

namespace IO = EVT::core::IO;
namespace time = EVT::core::time;

/** The address of the arduino listening for I2C requests */
constexpr uint8_t I2C_SLAVE_ADDR = 0x04;
/** The "register" location of the "o" byte */
constexpr uint8_t O_REGISTER = 0x00;
/** The "register" location of the "k" byte */
constexpr uint8_t K_REGISTER = 0x01;

int main() {
    IO::I2C& i2c = IO::getI2C<IO::Pin::PB_8, IO::Pin::PB_9>();
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);

    uart.printf("Starting I2C test\n\r");

    while (1) {
        uart.printf("Requesting first byte\n\r");
        uint8_t oValue = i2c.readReg(I2C_SLAVE_ADDR, O_REGISTER);
        uart.printf("Reading second bytes\n\r");
        uint8_t kValue = i2c.readReg(I2C_SLAVE_ADDR, K_REGISTER);

        uart.printf("Bytes Read: %c %c\n\r", oValue, kValue);

        // Wait half a second before repeating the test
        time::wait(500);
    }
}
