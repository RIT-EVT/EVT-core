/**
 * Example that handles I2C communication between an Arduino and a
 * EVT-Core device.
 *
 * The sample will read two bytes from the Arduino which should contain
 * the bytes "o" and "k".
 */
#include <stdint.h>

#include <EVT/io/I2C.hpp>
#include <EVT/io/UART.hpp>
#include <EVT/io/manager.hpp>
#include <EVT/utils/time.hpp>

namespace IO = EVT::core::IO;
namespace time = EVT::core::time;

/** The address of the arduino listening for I2C requests */
constexpr uint8_t I2C_SLAVE_ADDR = 0x48;
/** The "register" location of the "o" byte */
constexpr uint8_t O_REGISTER = 0x00;
/** The "register" location of the "k" byte */
constexpr uint8_t K_REGISTER = 0x01;

int main() {
    // Initialize system
    IO::init();

    IO::I2C& i2c = IO::getI2C<IO::Pin::PB_8, IO::Pin::PB_9>();
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);

    uart.printf("Starting I2C test\n\r");

    while (1) {
        uart.printf("Requesting first byte\n\r");

        // Read the value of 'o'
        uint8_t regAddr = 0x00;
        uint8_t buf[2];
        IO::I2C::I2CStatus status = i2c.readReg(I2C_SLAVE_ADDR, &regAddr, 1, buf, 2);
        if (status != IO::I2C::I2CStatus::OK) {
            uart.printf("Failed read 'o' register with I2C::I2CStatus: %d\n\r",
                        status);
            break;
        }

        uart.printf("Reading second bytes 0x%02x%02x\n\r", buf[0], buf[1]);

        uint64_t realTemp;
        uint64_t msb = buf[0];
        uint64_t lsb = buf[1];
        msb <<= 8;
        realTemp = (msb | lsb) * 78125;

        uart.printf("Temperature read: %d\n\r", realTemp);

        //
        // Read the value of 'k'
        //        uint8_t kValue;
        //        status = i2c.readReg(I2C_SLAVE_ADDR, K_REGISTER, &kValue);
        //        if (status != IO::I2C::I2CStatus::OK) {
        //            uart.printf("Failed read 'k' register with I2C::I2CStatus: %d\n\r",
        //                        status);
        //            break;
        //        }
        //
        //        uart.printf("Bytes Read: %c %c\n\r", oValue, kValue);

        //          uart.printf("Bytes Read: %x\n\r", oValue);   //Prosper's Debugger line
        // Wait half a second before repeating the test
        time::wait(1000);
    }
}
