#ifndef _EVT_LCD_
#define _EVT_LCD_

#include <EVT/io/GPIO.hpp>
#include <EVT/io/SPI.hpp>
#include <EVT/io/UART.hpp>
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
     *
     * @param[in] regSelect Register select pin
     * @param[in] reset Reset pin
     * @param[in] spi SPI class for communication
     * @param[in] bitMap bitmap to display to the LCD
     */
    LCD(EVT::core::IO::GPIO& regSelect, EVT::core::IO::GPIO& reset, EVT::core::IO::SPI& spi);

     *
     * @param[in] page Page address to write to
     * @param[in] colUp Bits to write to the page address
     * @param[in] colLow Bits to write to the column select
     */
    void drivePixel(uint8_t page, uint8_t colUp, uint8_t colLow, uint8_t data);

    /**
     * Clears the LCD, changes are mirrored in the bitmap
     *
     * @param[in] bitMap Bitmap to be displayed
     */
    void clearLCD(const uint8_t* bitMap);

    /**
     * Displays the map for diagnostic purposes
     *
     * @param[in] bitMap Bitmap to be displayed
     */
    void displayMap(const uint8_t* bitMap);

    void displayBitMap(uint8_t * bitMap, uint8_t bitMapWidth, uint8_t bitMapHeight, uint8_t page, uint8_t column);

    /// initializes the LCD for operation (must be called to use the LCD)
    void initLCD();

private:
    /** register select pin for the LCD */
    EVT::core::IO::GPIO& regSelect;
    /** reset pin for the lcd */
    EVT::core::IO::GPIO& reset;
    /** chip select pin for the LCD */
    //EVT::core::IO::GPIO& chipSelect; // TODO: Need to figure out purpose of this
    /** SPI port for the LCD controller */
    EVT::core::IO::SPI& spi;

};

}// namespace EVT::core::DEV

#endif
