/**
 * Example that handles SPI communication between an EVT-Core
 * device and the ADXL345 accelerometer
 *
 */
#include <stdint.h>

#include <EVT/io/GPIO.hpp>
#include <EVT/io/I2C.hpp>
#include <EVT/io/UART.hpp>
#include <EVT/io/manager.hpp>
#include <EVT/utils/time.hpp>

namespace IO = EVT::core::IO;
namespace time = EVT::core::time;

constexpr uint32_t SPI_SPEED = SPI_SPEED_4MHZ;// 4MHz

/** The address of the arduino listening for I2C requests */
constexpr uint8_t SINGLE_BYTE = 0x10;
constexpr uint8_t READ_MULTIPLE_BYTE = 0x11;
uint8_t BYTE_MULTIPLE[] = {0x40, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F};
constexpr uint8_t BYTE_MULTIPLE_LENGTH = 9;
constexpr uint8_t WRITE_REG = 0x20;
constexpr uint8_t WRITE_REG_BYTE = 0xF3;
constexpr uint8_t READ_REG = 0x28;

constexpr uint8_t deviceCount = 1;

IO::GPIO* devices[deviceCount];

int main() {
    // Initialize system
    IO::init();

    //CS: D10
    devices[0] = &IO::getGPIO<IO::Pin::SPI_CS>(EVT::core::IO::GPIO::Direction::OUTPUT);
    devices[0]->writePin(EVT::core::IO::GPIO::State::HIGH);

    //CLK: D13
    //MISO: D12
    //MOSI: D11
    IO::SPI& spi = IO::getSPI<IO::Pin::SPI_SCK, EVT::core::IO::Pin::SPI_MOSI, EVT::core::IO::Pin::SPI_MISO>(devices, deviceCount);

    spi.configureSPI(SPI_SPEED, SPI_MODE3, SPI_MSB_FIRST);

    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);

    uart.printf("Starting SPI test\n\r");
    uint8_t byte;
    while (1) {
        uart.printf("Test Start\n\r");
        spi.startTransmission(0);
        // write a single byte
        spi.write(SINGLE_BYTE);
        // read a single byte
        IO::SPI::SPIStatus status = spi.read(&byte);
        uart.printf("SPI Status: %i", status);

        spi.endTransmission(0);
        uart.printf("reading single byte %X\n\r", byte);

        // write an array of data
        spi.startTransmission(0);
        spi.write(BYTE_MULTIPLE, BYTE_MULTIPLE_LENGTH);
        spi.endTransmission(0);
        uart.printf("sending %u bytes\n\r", BYTE_MULTIPLE_LENGTH);

        // read an array of data
        uint8_t readData[BYTE_MULTIPLE_LENGTH];
        spi.startTransmission(0);
        spi.write(READ_MULTIPLE_BYTE);
        spi.read(readData, BYTE_MULTIPLE_LENGTH - 1);
        spi.endTransmission(0);

        uart.printf("reading %u bytes\n\r[", BYTE_MULTIPLE_LENGTH - 1);
        for (int i = 0; i < BYTE_MULTIPLE_LENGTH - 1; i++) {
            uart.printf("%X, ", readData[i]);
        }
        // uart.printf("\n\r\n\r");

        uart.printf("\n\rtesting combined register read and write\n\r");

        spi.writeReg(0, WRITE_REG, WRITE_REG_BYTE);
        uart.printf("sending register byte %X\n\r", SINGLE_BYTE);

        status = spi.readReg(0, READ_REG, &byte);
        uart.printf("SPI Status: %i", status);
        uart.printf("reading register byte %X\n\r\n\r", byte);

        // Wait five seconds before repeating the test
        time::wait(500);
    }
}
