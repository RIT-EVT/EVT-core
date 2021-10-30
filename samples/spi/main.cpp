/**
 * Example that handles SPI communication between an Arduino and a
 * EVT-Core device.
 *
 *
 */
#include <stdint.h>

#include <EVT/io/manager.hpp>
#include <EVT/utils/time.hpp>
#include <EVT/io/I2C.hpp>
#include <EVT/io/UART.hpp>
#include <EVT/io/GPIO.hpp>

namespace IO = EVT::core::IO;
namespace time = EVT::core::time;

constexpr uint32_t SPI_SPEED = 4000000; // 4MHz
/** The address of the arduino listening for I2C requests */
constexpr uint8_t SINGLE_BYTE = 0x48;
uint8_t BYTE_MULTIPLE[] = {0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F};
constexpr uint8_t BYTE_MULTIPLE_LENGTH = sizeof BYTE_MULTIPLE;

constexpr uint8_t deviceCount = 1;


IO::GPIO* devices[deviceCount];

int main() {
    // Initialize system
    IO::init();

    devices[0] = &IO::getGPIO<IO::Pin::PB_4>(EVT::core::IO::GPIO::Direction::OUTPUT);

    IO::SPI& spi = IO::getSPI<IO::Pin::PB_3, EVT::core::IO::Pin::PC_12, EVT::core::IO::Pin::PC_11>(devices, deviceCount);
    spi.configureSPI(SPI_SPEED, SPIMode0, SPI_MSB_FIRST);
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);

    uart.printf("Starting SPI test\n\r");

    while (1) {
        uart.printf("sending single byte");
        spi.write(0, SINGLE_BYTE);
        uart.printf("sending multiple bytes");
        spi.write(0, BYTE_MULTIPLE, BYTE_MULTIPLE_LENGTH);
        //uart.printf("sending single byte");
        //uart.printf("sending single byte");
//        uart.printf("Requesting first byte\n\r");
//        uint8_t oValue = i2c.readReg(I2C_SLAVE_ADDR, O_REGISTER);
//        uart.printf("Reading second bytes\n\r");
//        uint8_t kValue = i2c.readReg(I2C_SLAVE_ADDR, K_REGISTER);
//
//        uart.printf("Bytes Read: %c %c\n\r", oValue, kValue);
//
//        // Wait half a second before repeating the test
        time::wait(1000);
    }
}
