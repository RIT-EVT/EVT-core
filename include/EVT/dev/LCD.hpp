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
#define DISPLAYOFF 0xAE

namespace EVT::core::DEV {
/**
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

    /**
     * Writes data to the LCD
     *
     * @param[in] data the data to write to the LCD.
     */
    void dataWrite(uint8_t data);

    /**
     * Writes commands to the LCD.
     * These commands are used to tell the display how to handle the data written by dataWrite();
     *
     * @param[in] data the command to write to the LCD
     */
    void commandWrite(uint8_t data);

    /**
     * Displays the data to a singular column on the display. Because the display works in 8 bit vertical columns,
     * this is the most fine grained change you can have.
     *
     * @param[in] page Page address to write to
     * @param[in] colUp Bits to write to the page address
     * @param[in] colLow Bits to write to the column select
     * @param[in] data Bits to write to the LCD
     */
    void driveColumn(uint8_t page, uint8_t colUp, uint8_t colLow, uint8_t data);

    /**
     * Clears the LCD. Changes are not mirrored in the bitmap.
     */
    void clearLCD();

    /**
     * Clears only a certain area on the display screen.
     *
     * @param[in] width the width in pixels of the area to clear. Range: 0-127
     * @param[in] height the height in pixels of the area to clear. Range: 0-63
     * @param[in] page the page to start the clearing on. Range: 0-7.
     * @param[in] column the column to start clearing on. Range: 0-127
     */
    void clearArea(uint8_t width, uint8_t height, uint8_t page, uint8_t column);

    /**
     * Displays the map for diagnostic purposes
     *
     * @param[in] bitMap Bitmap to be displayed
     */
    void setEntireScreenBitMap(const uint8_t* bitMap);

    /**
     * Displays the given bit map at a certain height and width on the page.
     *
     * @param bitMap[in] the bitmap to display.
     * @param bitMapWidth[in] the width of the bitmap in pixels.
     * @param bitMapHeight[in] the height of the bitmap in pixels.
     * @param page[in] the page to draw the bitmap on. Range: 0-7.
     * @param column[in] the column to draw the bitmap on. Range:0-127.
     */
    void displayBitMapInArea(uint8_t* bitMap, uint8_t bitMapWidth, uint8_t bitMapHeight, uint8_t page, uint8_t column);

    /**
     * Writes text to the screen. Has options to wrap the text around the edge of the screen if needed.
     *
     * @param text[in] the text to write to the screen.
     * @param page[in] the page to write the text too. Range 0-7.
     * @param column[in] the column to write the text too. Range 0-127.
     * @param wrapText[in] whether or not the text should be wrapped around the edge of the screen.
     */
    void writeText(const char* text, uint8_t page, uint8_t column, bool wrapText);

    /**
     * Set the default section titles to be displayed.
     *
     * @param[in] newSectionTitles an array of section titles to display.
     */
    void setDefaultSections(char* const newSectionTitles[9]);

    /**
     * Displays the section headers. Only needs to be called once unless cleared.
     */
    void displaySectionHeaders();

    /**
     * Set the text for a certain section of the screen.
     *
     * @param[in] section the section number to set the text for.
     * @param[in] text the text to write into the section.
     */
    void setTextForSection(uint8_t section, const char* text);

    /**
     * Initializes the LCD for operation (must be called to use the LCD)
     */
    void initLCD();

private:
    /** The total width of the screen */
    static const uint8_t screenSizeX = 128;

    /** The total height of the screen */
    static const uint8_t screenSizeY = 64;

    /** THe total number of sections */
    static const uint8_t numberOfSections = 9;

    /** The total number of sections to display per row on the screen. Basically number of columns */
    static const uint8_t sectionsPerRow = 3;

    /** Register select pin for the LCD */
    EVT::core::IO::GPIO& regSelect;

    /** Reset pin for the LCD */
    EVT::core::IO::GPIO& reset;

    /** Chip select pin for the LCD */
    //EVT::core::IO::GPIO& chipSelect; // TODO: Need to figure out purpose of this

    /** SPI port for the LCD controller */
    EVT::core::IO::SPI& spi;

    /** The default section titles for the display */
    char* sectionTitles[9] = {
        "Not Set",
        "Not Set",
        "Not Set",
        "Not Set",
        "Not Set",
        "Not Set",
        "Not Set",
        "Not Set",
        "Not Set",
    };
};

}// namespace EVT::core::DEV

#endif
