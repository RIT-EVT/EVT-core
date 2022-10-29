#include <EVT/dev/LCD.hpp>

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
    this->commandWrite(0xAE);
    this->commandWrite(0xAE);
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

void LCD::drivePixel(uint8_t page, uint8_t colUp, uint8_t colLow, uint8_t data) {
    this->commandWrite(0xAE);         //Display OFF
    this->commandWrite(0x40);         //Display start address + 0x40

    this->commandWrite(0x40);         //line to start writing on (0 -> 64) moves set bits with it DO NOT CHANGE
    this->commandWrite(0xB0 + page);  //writes the page address (4 bits, 8 rows selected by values 0-7 )
    this->commandWrite(0x10 + colUp); //writes the first 4 bits of the column select (out of 8 bits)
    this->commandWrite(0x00 + colLow);//writes the second 4 bits of the column select (out)

    this->dataWrite(data);
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

void LCD::clearLCD(const uint8_t* bitMap) {
    unsigned int i, j;
    unsigned char page = 0xB0;

    this->commandWrite(0xAE);         //Display OFF
    this->commandWrite(0x40);         //Display start address + 0x40
    for (i = 0; i < 8; i++) {            //64 pixel display / 8 pixels per page = 8 pages
        this->commandWrite(page);     //send page address
        this->commandWrite(0x10);     //column address upper 4 bits + 0x10
        this->commandWrite(0x00);     //column address lower 4 bits + 0x00
        for (j = 0; j < 128; j++) {      //128 columns wide
            this->dataWrite(0x00); //write clear pixels
            bitMap++;                   // Advance the bitmap pointer by one. This means we can just grab the last one the next loop.
        }
        page++;                         //after 128 columns, go to next page
    }
    this->commandWrite(0xAF);
}

void LCD::clearArea(uint8_t width, uint8_t height, uint8_t page, uint8_t column){
    unsigned int i, j;
    uint8_t columnUpperAddress = column;
    columnUpperAddress >>= 4;

    uint8_t columnLowerAddress = column;
    columnLowerAddress <<= 4;
    columnLowerAddress >>= 4;

    int amountOfPages = height / 8;
    if (height < 8) amountOfPages = 1;

    this->commandWrite(0xAE);           //Display OFF
    this->commandWrite(0x40);           //Display start address + 0x40
    for (i = 0; i < amountOfPages; i++) { //64 pixel display / 8 pixels per page = 8 pages
        this->commandWrite(0xB0 + page);       //send page address
        this->commandWrite(0x10 + columnUpperAddress);       //column address upper 4 bits + 0x10
        this->commandWrite(0x00 + columnLowerAddress);       //column address lower 4 bits + 0x00

        for (j = 0; j < width; j++) {
            this->dataWrite(0x00);
        }

        page++; //after 128 columns, go to next page
    }
    this->commandWrite(0xAF);

}

void LCD::displayMap(const uint8_t* bitMap) {
    unsigned int i, j;
    unsigned char page = 0xB0;
    this->commandWrite(0xAE);           //Display OFF
    this->commandWrite(0x40);           //Display start address + 0x40
    for (i = 0; i < 8; i++) { //64 pixel display / 8 pixels per page = 8 pages
        this->commandWrite(page);       //send page address
        this->commandWrite(0x10);       //column address upper 4 bits + 0x10
        this->commandWrite(0x00);       //column address lower 4 bits + 0x00
        for (j = 0; j < 128; j++) {         //128 columns wide
            this->dataWrite(*bitMap);  //write pixels from bitmap
            bitMap++;                       // Advance the bitmap pointer by one. This means we can just grab the last one the next loop.
        }
        page++; //after 128 columns, go to next page
    }
    this->commandWrite(0xAF);
}

void LCD::displayBitMap(uint8_t * bitMap, uint8_t bitMapWidth, uint8_t bitMapHeight, uint8_t page, uint8_t column) {
    unsigned int i, j;
    uint8_t columnUpperAddress = column;
    columnUpperAddress >>= 4;

    uint8_t columnLowerAddress = column;
    columnLowerAddress <<= 4;
    columnLowerAddress >>= 4;

    int amountOfPages = bitMapHeight / 8;
    if (bitMapHeight < 8) amountOfPages = 1;

    this->commandWrite(0xAE);           //Display OFF
    this->commandWrite(0x40);           //Display start address + 0x40
    for (i = 0; i < amountOfPages; i++) { //64 pixel display / 8 pixels per page = 8 pages
        this->commandWrite(0xB0 + page);       //send page address
        this->commandWrite(0x10 + columnUpperAddress);       //column address upper 4 bits + 0x10
        this->commandWrite(0x00 + columnLowerAddress);       //column address lower 4 bits + 0x00

        for (j = 0; j < bitMapWidth; j++) {         //128 columns wide
            this->dataWrite(*bitMap);  //write pixels from bitmap
            bitMap++;                       // Advance the bitmap pointer by one. This means we can just grab the last one the next loop.
        }

        page++; //after 128 columns, go to next page
    }
    this->commandWrite(0xAF);

}
}// namespace EVT::core::DEV
