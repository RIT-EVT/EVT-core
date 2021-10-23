/**
 * Example that handles data reading and writing to an EEPROM.
 *
 * The sample will read and write two bytes to the EEPROM which
 * should contain the values "aa" and "bb".
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

int main() {
    // Initialize system
    IO::init();

    IO::I2C& i2c = IO::getI2C<IO::Pin::PB_8, IO::Pin::PB_9>();
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);

    uart.printf("Starting I2C test\n\r");


    i2c.writeMemReg(I2C_SLAVE_ADDR, 0x00, 0xaa, 2);
    i2c.writeMemReg(I2C_SLAVE_ADDR, 0x01, 0xbb, 2);

    //while (1) {
    uart.printf("Requesting first byte\n\r");
    uint8_t buffer = 0x00;
    uint8_t buffer2 = 0x00;
    uart.printf("Error: %#x\n\r", i2c.readMemReg(I2C_SLAVE_ADDR, 0x00, &buffer, 2));
    uart.printf("Error: %#x\n\r", i2c.readMemReg(I2C_SLAVE_ADDR, 0x01, &buffer2, 2));

    uart.printf("Byte Read: %#x\n\r", buffer);
    uart.printf("Byte Read: %#x\n\r", buffer2);

    // Wait 2 seconds before repeating the test
    time::wait(2000);
    //}
}
