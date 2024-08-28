/**
 * Sample code for displaying a moving ball animation on an LCD.
 */

#include <core/dev/LCD.hpp>
#include <core/dev/LED.hpp>
#include <core/io/UART.hpp>
#include <core/manager.hpp>
#include <core/utils/time.hpp>

namespace dev  = core::dev;
namespace io   = core::io;
namespace time = core::time;

constexpr uint32_t SPI_SPEED = SPI_SPEED_500KHZ;

constexpr uint8_t deviceCount = 1;

io::GPIO* devices[deviceCount];

int main() {
    // Initialize system
    core::platform::init();

    // Setup UART
    io::UART& uart = io::getUART<io::Pin::UART_TX, io::Pin::UART_RX>(9600);

    // Uses HUDL 1.0 Pins
    io::GPIO& regSelect = io::getGPIO<io::Pin::PA_3>(core::io::GPIO::Direction::OUTPUT);
    io::GPIO& reset     = io::getGPIO<io::Pin::PB_3>(core::io::GPIO::Direction::OUTPUT);

    devices[0] = &io::getGPIO<io::Pin::PB_12>(core::io::GPIO::Direction::OUTPUT);
    devices[0]->writePin(io::GPIO::State::HIGH);

    // Setup SPI
    io::SPI& spi = io::getSPI<io::Pin::SPI_SCK, io::Pin::SPI_MOSI>(devices, deviceCount);
    spi.configureSPI(SPI_SPEED, io::SPI::SPIMode::SPI_MODE0, SPI_MSB_FIRST);

    // Sets up LCD
    uart.printf("Creating LCD Object...\n\r");
    core::dev::LCD lcd(regSelect, reset, spi);
    uart.printf("Initializing LCD...\n\r");
    lcd.initLCD();
    lcd.clearLCD();

    uint8_t ball[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    uint8_t col     = 0;
    uint8_t page    = 0;

    lcd.displayBitMapInArea(ball, 8, 1, page, col);

    while (true) {
        lcd.clearArea(8, 1, page, col);

        col++;
        if (col >= 128) {
            col = 0;
            page++;

            if (page > 7) {
                page = 0;
            }
        }

        lcd.displayBitMapInArea(ball, 8, 1, page, col);

        time::wait(300);
    }

    return 0;
}
