/**
 * Example that handles SPI communication between an EVT-Core
 * device and the ADXL345 accelerometer
 *
 */

#include <core/io/GPIO.hpp>
#include <core/io/SPI.hpp>
#include <core/io/UART.hpp>
#include <core/manager.hpp>
#include <core/utils/time.hpp>

namespace io   = core::io;
namespace time = core::time;

constexpr uint32_t SPI_SPEED = SPI_SPEED_500KHZ; // 500KHz

constexpr uint8_t deviceCount = 1;

#define ADXL345_REG_POWER_CTL 0x2D // R/W   00000000  Power-saving features control  ----
#define ADXL345_REG_DATAY0    0x34 // R     00000000  Y-Axis Data 0

io::GPIO* devices[deviceCount];

int main() {
    // Initialize system
    core::platform::init();

    devices[0] = &io::getGPIO<io::Pin::SPI_CS>(io::GPIO::Direction::OUTPUT);
    devices[0]->writePin(io::GPIO::State::HIGH);

    io::SPI& spi = io::getSPI<io::Pin::SPI_SCK, io::Pin::SPI_MOSI, io::Pin::SPI_MISO>(devices, deviceCount);

    spi.configureSPI(SPI_SPEED, io::SPI::SPIMode::SPI_MODE3, SPI_MSB_FIRST);

    io::UART& uart = io::getUART<io::Pin::UART_TX, io::Pin::UART_RX>(9600);

    uart.printf("Starting SPI test\n\r");
    uint8_t byte = 0;
    while (byte != 0xE5) {
        io::SPI::SPIStatus status = spi.readReg(0, 0x00 | 0x80, &byte);
        if (status != io::SPI::SPIStatus::OK) {
            uart.printf("SPI readReg Error!\n\r");
        }

        uart.printf("device ID: 0x%X, %d\n\r", byte, byte == 0xE5); // should be 0xE5
        time::wait(500);
    }
    spi.writeReg(0, ADXL345_REG_POWER_CTL, 0x08);

    int16_t data;
    uint8_t bytes[2];
    while (1) {
        spi.startTransmission(0);
        spi.write(ADXL345_REG_DATAY0 | 0x80 | 0x40);
        spi.read(bytes, 2);
        data = bytes[0] | bytes[1] << 8;
        spi.endTransmission(0);
        uart.printf("Y: %i\n\r", data);

        // Wait half a second before repeating the test
        time::wait(500);
    }
}
