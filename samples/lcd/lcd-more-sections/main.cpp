/**
* Sample code for display data on an LCD with more than the default sections
*/

#include <core/dev/LCD.hpp>
#include <core/dev/LED.hpp>
#include <core/io/UART.hpp>
#include <core/manager.hpp>
#include <core/utils/time.hpp>
#include <cstdio>

namespace DEV = core::DEV;
namespace IO = core::IO;
namespace time = core::time;

constexpr uint32_t SPI_SPEED = SPI_SPEED_500KHZ;

constexpr uint8_t deviceCount = 1;

IO::GPIO* devices[deviceCount];

int main() {
    // Initialize system
    core::platform::init();

    // Setup UART
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);

    // Uses HUDL 1.0 Pins
    IO::GPIO& regSelect = IO::getGPIO<IO::Pin::PA_3>(core::IO::GPIO::Direction::OUTPUT);
    IO::GPIO& reset = IO::getGPIO<IO::Pin::PB_3>(core::IO::GPIO::Direction::OUTPUT);
    devices[0] = &IO::getGPIO<IO::Pin::PB_12>(core::IO::GPIO::Direction::OUTPUT);
    devices[0]->writePin(IO::GPIO::State::HIGH);

    // Setup SPI
    IO::SPI& spi = IO::getSPI<IO::Pin::SPI_SCK, IO::Pin::SPI_MOSI>(devices, deviceCount);
    spi.configureSPI(SPI_SPEED, IO::SPI::SPIMode::SPI_MODE0, SPI_MSB_FIRST);

    // Sets up LCD
    uart.printf("Creating LCD Object...\n\r");
    core::DEV::LCD lcd(regSelect, reset, spi, 12, 3);
    uart.printf("Initializing LCD...\n\r");
    lcd.initLCD();
    lcd.clearLCD();

    // The segment titles
    char* titles[12] = {
        (char*) "B Voltage",
        (char*) "Speed",
        (char*) "RPM",
        (char*) "Temp 1",
        (char*) "Temp 2",
        (char*) "Temp 3",
        (char*) "Status 1",
        (char*) "PVC Stat",
        (char*) "Torque",
        (char*) "Batt Stat",
        (char*) "HUDL Stat",
        (char*) "PVC Stat",
    };
    lcd.setDefaultSections(titles);

    // Set the default
    lcd.displaySectionHeaders();
    lcd.setTextForSection(0, "3.2 v");
    lcd.setTextForSection(1, "25 MPH");
    lcd.setTextForSection(2, "3000");
    lcd.setTextForSection(3, "40 C");
    lcd.setTextForSection(4, "44 C");
    lcd.setTextForSection(5, "43 C");
    lcd.setTextForSection(6, "ON");
    lcd.setTextForSection(7, "Ready");
    lcd.setTextForSection(8, "100 NM");
    lcd.setTextForSection(9, "Ready");
    lcd.setTextForSection(10, "Ready");
    lcd.setTextForSection(11, "Ready");

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
