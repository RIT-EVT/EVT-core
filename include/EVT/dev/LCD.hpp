#ifndef _EVT_LCD_
#define _EVT_LCD_
#endif

#include <EVT/io/GPIO.hpp>
#include <EVT/io/SPI.hpp>
#include <EVT/io/pin.hpp>
#include <EVT/io/manager.hpp>
#include <stdint.h>

namespace EVT::core::DEV {
/*
* This class represents the structure to command a GLCD with 
* a ST7565 controller.
*/
class LCD {

public:
    /// represents different commands to send to the LCD
    enum class Command{
        ADCSELECT = 160u,
        DISPLAYOFF = 174u,
        COMDIRSCAN = 200u,
        LCDBIASET = 162u,
        POWERCONTROLSET = 47u,
        RESRATIOSET = 38u,
        ELECTRONICVOLCOMMAND = 129u,
        ELECTRONICVOLVALUE = 11u,
        DISPLAYON = 175u
    };

    /// bit map to display the LCD's current state
    unsigned char * bitMap;

    /// 
    /*
    * Constructor for the LCD class
    *
    * @param[in]
    *
    */
    LCD(EVT::core::IO::GPIO& regSelect,EVT::core::IO::GPIO& reset,EVT::core::IO::SPI& spi, unsigned char * bitMap);

    /// function to write data to the LCD
    void dataWrite(unsigned char data);

   /// writes commands to the LCD 
    void commWrite(unsigned char data);

    /// drives a particular pixel on the LCD
    void drivePixel(unsigned char page, unsigned char col_up, unsigned char col_low, unsigned char data);

    /// clears the LCD, changes are mirrores in the bitmap
    void clearLCD(unsigned char * bitMap);

    /// displays the map for diagnostic purposes
    void displayMap(unsigned char * bitMap);

    /// initializes the LCD for operation (must be called to use the LCD)
    void initLCD();

private:
    /// register select pin for the LCD
    EVT::core::IO::GPIO& regSelect;
    /// reset pin for the lcd
    EVT::core::IO::GPIO& reset;
    /// chip select pin for the LCD
    //EVT::core::IO::GPIO& chipSelect;
    /// SPI port for the LCD controller 
    EVT::core::IO::SPI& spi; 
    /// command getting send to the controller
    Command command;
};

}

