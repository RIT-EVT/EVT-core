#ifndef _EVT_LCD_
#define _EVT_LCD_

#include <EVT/io/GPIO.hpp>
#include <stdint.h>

namespace EVT::core::DEV {

class LCD {
public:
    /// represents different commands to send to the LCD
    enum class Command {
        ADCSELECT = 160;
        DISPLAYOFF = 174;
        COMDIRSCAN = 200;
        LCDBIASET = 162;
        POWERCONTROLSET = 47;
        RESRATIOSET = 38;
        ELECTRONICVOLCOMMAND = 129;
        ELECTRONICVOLVALUE = 11;
        DISPLAYON = 175;
    };

    /// bit map to display the LCD's current state
    uint8_t * bitMap;

    /// constructor for the LCD class
    LCD(EVT::core::IO::GPIO& regSelect,EVT::core::IO::GPIO& reset,EVT::core::IO::GPIO& chipSelect,EVT::core::IO::SPI& spi, uint8_t * bitMap);

    /// function to write data to the LCD
    void dataWrite(uint8_t data);

   /// writes commands to the LCD 
    void commWrite(uint8_t data);

    /// drives a particular pixel on the LCD
    void drivePixel(uint8_t data);

    /// clears the LCD, changes are mirrores in the bitmap
    void clearLCD(uint8_t * bitMap);

    /// displays the map for diagnostic purposes
    void displayMap(uint8_t * bitMap)

    /// initializes the LCD for operation (must be called to use the LCD)
    void initLCD(Command command);

private:
    /// register select pin for the LCD
    EVT::core::IO::GPIO& regSelect;
    /// reset pin for the lcd
    EVT::core::IO::GPIO& reset;
    /// chip select pin for the LCD
    EVT::core::IO::GPIO& chipSelect;
    /// SPI port for the LCD controller 
    EVT::core::IO::SPI& spi; 
    /// command getting send to the controller
    Command command;
}


}