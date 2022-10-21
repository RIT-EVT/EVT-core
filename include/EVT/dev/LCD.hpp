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



    /**
     * Constructor the LCD class
     * @param regSelect - register select pin
     * @param reset - reset pin
     * @param spi - SPI class for communication
     * @param bitMap - bitmap to display to the LCD
     */
    LCD(EVT::core::IO::GPIO& regSelect,EVT::core::IO::GPIO& reset,EVT::core::IO::SPI& spi);

    /**
     * Writes data to the LCD
     * @param data - data to write to the LCD
     */
    void dataWrite(uint8_t data);

   /**
    * Writes commands to the LCD
    * @param data -
    */
    void commandWrite(uint8_t data);

    /**
     * Drives a particular pixel on the LCD
     * @param page
     * @param col_up
     * @param col_low
     * @param data
     */
    void drivePixel(uint8_t page, uint8_t col_up, uint8_t col_low, uint8_t data);

    /**
     * Clears the LCD, changes are mirrored in the bitmap
     * @param bitMap
     */
    void clearLCD(const uint8_t * bitMap);

    /**
     * Displays the map for diagnostic purposes
     * @param bitMap
     */
    void displayMap(const uint8_t * bitMap);

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
    /** bit map to display the LCD's current state */
    uint8_t * bitMap;
};

}// namespace EVT::core::DEV

#endif

