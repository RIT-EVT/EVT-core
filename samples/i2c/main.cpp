/**
 * Example that handles I2C communication between an Arduino and a
 * EVT-Core device.
 *
 * The sample will read two bytes from the Arduino which should contain
 * the bytes "o" and "k".
 */
#include <stdint.h>

#include <core/io/I2C.hpp>
#include <core/io/UART.hpp>
#include <core/manager.hpp>
#include <core/utils/time.hpp>

namespace io   = core::io;
namespace time = core::time;

/** The address of the arduino listening for I2C requests */
constexpr uint8_t I2C_SLAVE_ADDR = 0x04;
/** The "register" location of the "o" byte */
constexpr uint8_t O_REGISTER = 0x00;
/** The "register" location of the "k" byte */
constexpr uint8_t K_REGISTER = 0x01;

int main() {
    // Initialize system
    core::platform::init();

    io::I2C& i2c   = io::getI2C<io::Pin::PB_8, io::Pin::PB_9>();
    io::UART& uart = io::getUART<io::Pin::UART_TX, io::Pin::UART_RX>(9600);

    uart.printf("Starting I2C test\n\r");

    while (1) {
        uart.printf("Requesting first byte\n\r");

        // Read the value of 'o'
        uint8_t oValue;
        io::I2C::I2CStatus status = i2c.readReg(I2C_SLAVE_ADDR, O_REGISTER, &oValue);
        if (status != io::I2C::I2CStatus::OK) {
            uart.printf("Failed read 'o' register with I2C::I2CStatus: %d\n\r", status);
            break;
        }

        uart.printf("Reading second bytes\n\r");

        // Read the value of 'k'
        uint8_t kValue;
        status = i2c.readReg(I2C_SLAVE_ADDR, K_REGISTER, &kValue);
        if (status != io::I2C::I2CStatus::OK) {
            uart.printf("Failed read 'k' register with I2C::I2CStatus: %d\n\r", status);
            break;
        }

        uart.printf("Bytes Read: %c %c\n\r", oValue, kValue);

        // Wait half a second before repeating the test
        time::wait(500);
    }
}
