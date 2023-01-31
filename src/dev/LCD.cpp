#include <EVT/dev/BitmapFonts.hpp>
#include <EVT/dev/LCD.hpp>
#include <cstring>

namespace EVT::core::DEV {
LCD::LCD(EVT::core::IO::GPIO& regSelect, EVT::core::IO::GPIO& reset, EVT::core::IO::SPI& spi)
    : regSelect(regSelect), reset(reset), spi(spi) {
    this->regSelect.writePin(EVT::core::IO::GPIO::State::LOW);
    this->reset.writePin(EVT::core::IO::GPIO::State::LOW);
}

void LCD::initLCD() {
    this->reset.writePin(EVT::core::IO::GPIO::State::HIGH);
    EVT::core::time::wait(100);
    this->commandWrite(ADCSELECT);           // ADC select
    this->commandWrite(DISPLAYOFF);          // Display OFF
    this->commandWrite(COMDIRSCAN);          // COM direction scan
    this->commandWrite(LCDBIASET);           // LCD bias set
    this->commandWrite(POWERCONTROLSET);     // Power Control set
    this->commandWrite(RESRATIOSET);         // Resistor Ratio Set
    this->commandWrite(ELECTRONICVOLCOMMAND);// Electronic Volume Command (set contrast) Double Byte: 1 of 2
    this->commandWrite(ELECTRONICVOLVALUE);  // Electronic Volume value (contrast value) Double Byte: 2 of 2
    this->commandWrite(DISPLAYON);           // Display ON
}

void LCD::dataWrite(uint8_t data) {
    this->regSelect.writePin(EVT::core::IO::GPIO::State::HIGH);
    this->spi.startTransmission(0);
    this->spi.write(&data, 1);
    this->spi.endTransmission(0);
}

void LCD::commandWrite(uint8_t data) {
    this->regSelect.writePin(EVT::core::IO::GPIO::State::LOW);
    this->spi.startTransmission(0);
    this->spi.write(&data, 1);
    this->spi.endTransmission(0);
}

void LCD::driveColumn(uint8_t page, uint8_t colUp, uint8_t colLow, uint8_t data) {
    this->commandWrite(0x40);         // Line to start writing on (0 -> 64) moves set bits with it DO NOT CHANGE
    this->commandWrite(0xB0 + page);  // Writes the page address (4 bits, 8 rows selected by values 0-7 )
    this->commandWrite(0x10 + colUp); // Writes the first 4 bits of the column select (out of 8 bits)
    this->commandWrite(0x00 + colLow);// Writes the second 4 bits of the column select (out)

    this->dataWrite(data);

    this->commandWrite(0xAF);// Finish Writing
    /*
     * writes 8 vertical bits based on value between 0-255 based on bits set ex: 01001100(0x4C) is
     * |WHITE|
     * |BLACK|
     * |WHITE|
     * |WHITE|
     * |BLACK|
     * |BLACK|
     * |WHITE|
     * |WHITE|
     */
}

void LCD::clearLCD() {
    uint8_t page = 0xB0;// The starting page

    this->commandWrite(0x40);                      // Display start address + 0x40
    for (uint8_t i = 0; i < 8; i++) {              // 64 pixel display / 8 pixels per page = 8 pages
        this->commandWrite(page);                  // Send page address
        this->commandWrite(0x10);                  // Column address upper 4 bits + 0x10
        this->commandWrite(0x00);                  // Column address lower 4 bits + 0x00
        for (uint8_t j = 0; j < screenSizeX; j++) {// 128 columns wide
            this->dataWrite(0x00);                 // Write clear pixels
        }
        page++;// After 128 columns, go to next page
    }
    this->commandWrite(0xAF);
}

void LCD::clearArea(uint8_t width, uint8_t height, uint8_t page, uint8_t column) {
    // Calculate the correct column address from an 8-bit number.
    // The issue is that the display takes 4 bit numbers, instead of 8.
    // So you need to split it into two 4 bit numbers.
    uint8_t columnUpperAddress = (column & 0xf0) >> 4;
    uint8_t columnLowerAddress = (column & 0x0f);

    uint8_t amountOfPages = height / 8;
    if (height < 8) {
        amountOfPages = 1;
    }

    this->commandWrite(0x40);                         // Display start address + 0x40
    for (uint8_t i = 0; i < amountOfPages; i++) {     // 64 pixel display / 8 pixels per page = 8 pages
        this->commandWrite(0xB0 + page);              // Send page address
        this->commandWrite(0x10 + columnUpperAddress);// Column address upper 4 bits + 0x10
        this->commandWrite(0x00 + columnLowerAddress);// Column address lower 4 bits + 0x00

        for (uint8_t j = 0; j < width; j++) {
            this->dataWrite(0x00);// Write Clear Pixels
        }

        page++;// After 128 columns, go to next page
    }
    this->commandWrite(0xAF);// Finish Writing
}

void LCD::setEntireScreenBitMap(const uint8_t* bitMap) {
    uint8_t page = 0xB0;
    this->commandWrite(0x40);              //Display start address + 0x40
    for (uint8_t i = 0; i < 8; i++) {      //64 pixel display / 8 pixels per page = 8 pages
        this->commandWrite(page);          //send page address
        this->commandWrite(0x10);          //column address upper 4 bits + 0x10
        this->commandWrite(0x00);          //column address lower 4 bits + 0x00
        for (uint8_t j = 0; j < 128; j++) {//128 columns wide
            this->dataWrite(*bitMap);      //write pixels from bitmap
            bitMap++;                      // Advance the bitmap pointer by one. This means we can just grab the last one the next loop.
        }
        page++;//after 128 columns, go to next page
    }
    this->commandWrite(0xAF);
}

void LCD::displayBitMapInArea(uint8_t* bitMap, uint8_t bitMapWidth, uint8_t bitMapHeight, uint8_t page, uint8_t column) {
    // Calculate the correct column address from an 8-bit number.
    // The issue is that the display takes 4 bit numbers, instead of 8.
    // So you need to split it into two 4 bit numbers.
    uint8_t columnUpperAddress = (column & 0xf0) >> 4;
    uint8_t columnLowerAddress = (column & 0x0f);

    uint8_t amountOfPages = bitMapHeight / 8;
    if (bitMapHeight < 8) {
        amountOfPages = 1;
    }

    this->commandWrite(0x40);                         //Display start address + 0x40
    for (uint8_t i = 0; i < amountOfPages; i++) {     // 64 pixel display / 8 pixels per page = 8 pages
        this->commandWrite(0xB0 + page);              // Send page address
        this->commandWrite(0x10 + columnUpperAddress);// Column address upper 4 bits + 0x10
        this->commandWrite(0x00 + columnLowerAddress);// Column address lower 4 bits + 0x00

        for (uint8_t j = 0; j < bitMapWidth; j++) {
            this->dataWrite(*bitMap);// Write pixels from bitmap
            bitMap++;                // Advance the bitmap pointer by one. This means we can just grab the last one the next loop.
        }

        page++;// After 128 columns, go to next page
    }
    this->commandWrite(0xAF);// Finish writing
}

void LCD::writeText(const char* text, uint8_t page, uint8_t column, bool wrapText) {
    for (uint8_t x = 0; x < strlen(text); x++) {
        // Get the ASCII value of the character.
        uint8_t fontIndex = text[x];

        // Create the character that we need to write to the screen.
        unsigned char characterMap[4] = {
            BitmapFont::font4x6[fontIndex][0],
            BitmapFont::font4x6[fontIndex][1],
            BitmapFont::font4x6[fontIndex][2],
            BitmapFont::font4x6[fontIndex][3],
        };

        if (column >= screenSizeX) {
            return;
        }

        // Display the character bit map at the calculated page and column.
        displayBitMapInArea(characterMap, 4, 8, page, column);
        column += 4;// Advance the column for the next character.

        // If we need to wrap text, move the page forward and the column to 0.
        if (wrapText && column >= screenSizeX) {
            page++;
            column = 0;
        }
    }
}

void LCD::setDefaultSections(char* newSectionTitles[9]) {
    for (uint8_t x = 0; x < numberOfSections; x++) {
        this->sectionTitles[x] = newSectionTitles[x];
    }
}

void LCD::displaySectionHeaders() {
    uint8_t page = 0;
    uint8_t column = 0;
    uint8_t rowCounter = 0;

    uint8_t sectionWidth = screenSizeX / sectionsPerRow;

    for (auto title : sectionTitles) {
        uint8_t length = strlen(title) * 4;// The length of the text, multiply by 4 because each character is 4 pixels wide.
        uint8_t padding = (sectionWidth - length) / 2;

        column += padding;

        // Write the text for the section header to the screen.
        writeText(title, page, column, false);

        // Increment counters for the next section.
        column += length;
        column += padding;

        // Check if we need to go to the next page.
        rowCounter++;
        if (rowCounter >= sectionsPerRow) {
            rowCounter = 0;
            column = 0;
            page += 3;
        }

        // Check if we ran out of screen space.
        if (column >= screenSizeX) {
            column = 0;
        }
    }
}

void LCD::setTextForSection(uint8_t section, const char* text) {
    uint8_t sectionWidth = screenSizeX / sectionsPerRow;
    uint8_t adjustedSection = section + 1;        // Adjust the section so the following math operates correctly.
    uint8_t sectionRow = section / sectionsPerRow;// Calculate the correct row for the section

    uint8_t sectionPage = (sectionRow * 3) + 1;
    uint8_t sectionColumn = (adjustedSection - (sectionRow * sectionsPerRow) - 1) * sectionWidth;// Calculate what the column # for the section is.

    // Clear the sections area so text is not written over old text.
    clearArea(sectionWidth, 8, sectionPage, sectionColumn);

    // Calculate the padding to center the text in the section
    uint8_t length = strlen(text) * 4;
    uint8_t padding = (sectionWidth - length) / 2;

    sectionColumn += padding;

    // Write the text to the screen under the section header.
    writeText(text, sectionPage, sectionColumn, false);
}

}// namespace EVT::core::DEV
