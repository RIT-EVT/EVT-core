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
constexpr uint8_t I2C_SLAVE_ADDR = 0x50;
/** The "register" location of the "o" byte */
constexpr uint8_t O_REGISTER = 0x12;

int main() {
    // Initialize system
    IO::init();

    IO::I2C& i2c = IO::getI2C<IO::Pin::PB_8, IO::Pin::PB_9>();
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);

    uart.printf("Starting I2C test\n\r");

    //i2c.writeReg(I2C_SLAVE_ADDR, O_REGISTER, 0xfa);
    uint8_t reg[] = {0x00,0x00, 0xfa};
    i2c.write(I2C_SLAVE_ADDR, reg, 3);

    while (1) {


        uart.printf("Requesting first byte\n\r");
        uint8_t regOnly[] = {0x00,0x00};
        i2c.write(I2C_SLAVE_ADDR, regOnly, 2);
        uint8_t oValue = i2c.read(I2C_SLAVE_ADDR);

        uart.printf("Byte Read: %#x\n\r", oValue);

        // Wait half a second before repeating the test
        time::wait(2000);
    }
}
