/**
 * Sample code for showing all capabilities of an LCD.
 */

#include <core/dev/LCD.hpp>
#include <core/dev/LED.hpp>
#include <core/io/UART.hpp>
#include <core/manager.hpp>
#include <core/utils/time.hpp>
#include <cstdio>

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

    const char* text =
        R"( !"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~)";
    lcd.writeText(text, 0, 0, core::dev::LCD::SMALL, true);

    uint8_t ball[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    uint8_t col     = 0;
    uint8_t page    = 4;

    lcd.displayBitMapInArea(ball, 8, 1, page, col);

    uint8_t number = 0;

    while (true) {
        lcd.clearArea(8, 1, page, col);

        col++;
        if (col >= 128) {
            col = 0;
            page++;

            if (page > 6) {
                page = 4;
            }
        }

        lcd.displayBitMapInArea(ball, 8, 1, page, col);

        lcd.clearArea(16, 1, 7, 0);

        char buffer[128] = {};
        snprintf(buffer, (8), "%d", (number));
        lcd.writeText(buffer, 7, 0, core::dev::LCD::SMALL, true);

        number++;
        time::wait(500);
    }

    return 0;
}
