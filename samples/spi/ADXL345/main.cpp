/**
 * Example that handles SPI communication between an EVT-Core
 * device and the ADXL345 accelerometer
 *
 */
#include <stdint.h>

#include <EVT/io/GPIO.hpp>
#include <EVT/io/I2C.hpp>
#include <EVT/io/UART.hpp>
#include <EVT/manager.hpp>
#include <EVT/utils/time.hpp>

namespace IO = EVT::core::IO;
namespace time = EVT::core::time;

constexpr uint32_t SPI_SPEED = SPI_SPEED_62KHZ;// 62.5KHz

constexpr uint8_t deviceCount = 1;

#define ADXL345_REG_POWER_CTL 0x2D//R/W   00000000  Power-saving features control  ----
#define ADXL345_REG_DATAY0 0x34   //R     00000000  Y-Axis Data 0

IO::GPIO* devices[deviceCount];

int main() {
    // Initialize system
    EVT::core::platform::init();

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
    uint8_t byte = 0;
    while (byte != 0xE5) {
        IO::SPI::SPIStatus status = spi.readReg(0, 0x00 | 0x80, &byte);
        uart.printf("device ID: 0x%X, %d\n\r", byte, byte == 0xE5);//should be 0xE5
        time::wait(500);
    }
    spi.writeReg(0, ADXL345_REG_POWER_CTL, 0x08);

    int16_t data;
    uint8_t bytes[2];
    while (1) {
        //data = spi.readReg(0, ADXL345_REG_DATAY0 | 0x80 | 0x40);

        spi.startTransmission(0);
        spi.write(ADXL345_REG_DATAY0 | 0x80 | 0x40);
        spi.read(bytes, 2);
        data = bytes[0] | bytes[1] << 8;
        spi.endTransmission(0);
        uart.printf("Y: %i\n\r", data);

        // Wait five seconds before repeating the test
        time::wait(500);
    }
}
