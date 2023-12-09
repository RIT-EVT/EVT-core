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
#include <EVT/manager.hpp>
#include <EVT/utils/time.hpp>
#include <EVT/io/pin.hpp>

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
    EVT::core::platform::init();

    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);

    uart.printf("no I2C test\n\r");

//    HAL_Init();

    IO::I2C& i2c = IO::getI2C<IO::Pin::I2C_SCL, IO::Pin::I2C_SDA>();


    uart.printf("Starting I2C test\n\r");


//    while (1) {
        uart.printf("Requesting first byte\n\r");

        // Read the value of 'o'
        uint8_t temp[2];
        uint8_t reg = 0x00;
        IO::I2C::I2CStatus status = i2c.readReg(I2C_SLAVE_ADDR, &reg, 1, temp, 2);
        if (status != IO::I2C::I2CStatus::OK) {
            uart.printf("Failed read 'o' register with I2C::I2CStatus: %d\n\r",
                        status);
//            break;
        }

        uart.printf("O value: %#x, %#x\n\r", temp[0], temp[1]);

        // Wait half a second before repeating the test
        time::wait(500);
//        break;
//    }
    return 0;
}
