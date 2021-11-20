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

    devices[0] = &IO::getGPIO<IO::Pin::PB_4>(EVT::core::IO::GPIO::Direction::OUTPUT);
    devices[0]->writePin(EVT::core::IO::GPIO::State::HIGH);

    IO::SPI& spi = IO::getSPI<IO::Pin::PB_3, EVT::core::IO::Pin::PC_12, EVT::core::IO::Pin::PC_11>(devices, deviceCount);
    spi.configureSPI(SPI_SPEED, SPIMode0, SPI_MSB_FIRST);
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);

    uart.printf("Starting SPI test\n\r");
    uint8_t byte;

    while (1) {
    //     uart.printf("Begin test\n\r");
    //     //write a single byte
    //     spi.write(0, SINGLE_BYTE);
    //     uart.printf("sending single byte %X\n\r", SINGLE_BYTE);

    //     //read a single byte
    //     byte = spi.read(0);
    //     uart.printf("reading single byte %X\n\r", byte);

    //    //write an array of data
    //    spi.write(0, BYTE_MULTIPLE, BYTE_MULTIPLE_LENGTH);
    //    uart.printf("sending %u bytes\n\r", BYTE_MULTIPLE_LENGTH);

    //    //read an array of data
    //    uint8_t readData[BYTE_MULTIPLE_LENGTH];
    //    spi.write(0, READ_MULTIPLE_BYTE);
    //    spi.read(0, readData, BYTE_MULTIPLE_LENGTH-1);
    //    uart.printf("reading %u bytes\n\r[", BYTE_MULTIPLE_LENGTH-1);
    //    for(int i = 0; i<BYTE_MULTIPLE_LENGTH; i++){
    //        uart.printf("%X, ", readData[i]);
    //    }

       uart.printf("\n\rtesting combined register read and write\n\r");

       spi.writeReg(0, WRITE_REG, WRITE_REG_BYTE);
       uart.printf("sending register byte %X\n\r", SINGLE_BYTE);

       byte = spi.readReg(0, READ_REG);
       uart.printf("reading register byte %X\n\r\n\r", byte);

        // Wait five seconds before repeating the test
        time::wait(1000);
    }
}
