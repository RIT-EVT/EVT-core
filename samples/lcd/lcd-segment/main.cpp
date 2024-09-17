/**
 * Sample code for displaying a segmented display on an LCD.
 */

#include <cstdio>

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

    // The segment titles
    char* titles[9] = {
        (char*) "B Voltage",
        (char*) "Speed",
        (char*) "RPM",
        (char*) "Temp 1",
        (char*) "Temp 2",
        (char*) "Temp 3",
        (char*) "Status 1",
        (char*) "PVC Stat",
        (char*) "Torque",
    };
    lcd.setDefaultSections(titles);

    // Set the default
    lcd.displaySectionHeaders();
    lcd.setTextForSection(0, "3.2 v");
    lcd.setTextForSection(2, "25 MPH");
    lcd.setTextForSection(2, "3000");
    lcd.setTextForSection(3, "40 C");
    lcd.setTextForSection(4, "44 C");
    lcd.setTextForSection(5, "43 C");
    lcd.setTextForSection(6, "ON");
    lcd.setTextForSection(7, "Ready");
    lcd.setTextForSection(8, "100 NM");

    uint8_t number = 0;

    while (true) {
        char buffer[128] = {};
        snprintf(buffer, (8), "%d", (number));
        lcd.setTextForSection(1, buffer);

        number++;
        time::wait(500);
    }
    return 0;
}
