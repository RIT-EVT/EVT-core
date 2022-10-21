#ifndef _EVT_LCD_
#define _EVT_LCD_

#include <EVT/io/GPIO.hpp>
#include <EVT/io/UART.hpp>
#include <EVT/io/SPI.hpp>
#include <EVT/io/pin.hpp>
#include <EVT/utils/time.hpp>
#include <stdint.h>

#define ADCSELECT 0xA0
#define DISPLAYOFF 0xAE
#define COMDIRSCAN 0xC8
#define LCDBIASET 0xA2
#define POWERCONTROLSET 0x2F
#define RESRATIOSET 0x26
#define ELECTRONICVOLCOMMAND 0x81
#define ELECTRONICVOLVALUE 0x11
#define DISPLAYON 0xAF

namespace EVT::core::DEV {
/*
* This class represents the structure to command a GLCD with 
* a ST7565 controller.
*/
class LCD {

public:

    /// bit map to display the LCD's current state
    unsigned char * bitMap;

    /**
     * Constructor the LCD class
     * @param regSelect - register select pin
     * @param reset - reset pin
     * @param spi - SPI class for communication
     * @param bitMap - bitmap to display to the LCD
     */
    LCD(EVT::core::IO::GPIO& regSelect,EVT::core::IO::GPIO& reset,EVT::core::IO::SPI& spi, unsigned char * bitMap);

    /**
     * Writes data to the LCD
     * @param data
     */
    void dataWrite(unsigned char data);

   /**
    * Writes commands to the LCD
    * @param data
    */
    void commandWrite(unsigned char data);

    /**
     * Drives a particular pixel on the LCD
     * @param page
     * @param col_up
     * @param col_low
     * @param data
     */
    void drivePixel(unsigned char page, unsigned char col_up, unsigned char col_low, unsigned char data);

    /**
     * Clears the LCD, changes are mirrored in the bitmap
     * @param bitMap
     */
    void clearLCD(unsigned char * bitMap);

    /**
     * Displays the map for diagnostic purposes
     * @param bitMap
     */
    void displayMap(unsigned char * bitMap);

    /**
     * Initializes the LCD for operation (must be called to use the LCD)
     */
    void initLCD();

private:
    /** register select pin for the LCD */
    EVT::core::IO::GPIO& regSelect;
    /** reset pin for the lcd */
    EVT::core::IO::GPIO& reset;
    /** chip select pin for the LCD */
    //EVT::core::IO::GPIO& chipSelect;
    /** SPI port for the LCD controller */
    EVT::core::IO::SPI& spi; 
};

}// namespace EVT::core::DEV

#endif

