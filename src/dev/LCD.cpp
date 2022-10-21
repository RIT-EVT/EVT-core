#include <EVT/dev/LCD.hpp>

namespace EVT::core::DEV {

LCD::LCD(EVT::core::IO::GPIO& regSelect, EVT::core::IO::GPIO& reset, EVT::core::IO::SPI& spi)
    : regSelect(regSelect), reset(reset), spi(spi) {
    this->regSelect.writePin(EVT::core::IO::GPIO::State::LOW);
    this->reset.writePin(EVT::core::IO::GPIO::State::LOW);
    //this->chipSelect.writePin(EVT::core::IO::GPIO::State::LOW);
    this->bitMap = {};
}

void LCD::dataWrite(uint8_t data) {

    //this->CS.writePin(EVT::core::IO::GPIO::State::LOW);
    this->regSelect.writePin(EVT::core::IO::GPIO::State::HIGH);
    this->spi.startTransmission(0);
    this->spi.write(&data, 1);
    this->spi.endTransmission(0);
    //this->CS.writePin(EVT::core::IO::GPIO::State::HIGH);
}

void LCD::commandWrite(uint8_t data) {
    //this->CS.writePin(EVT::core::IO::GPIO::State::LOW);
    this->regSelect.writePin(EVT::core::IO::GPIO::State::LOW);
    this->spi.startTransmission(0);
    this->spi.write(&data, 1);
    this->spi.endTransmission(0);
    //this->CS.writePin(EVT::core::IO::GPIO::State::HIGH);
}

void LCD::drivePixel(uint8_t page, uint8_t col_up, uint8_t col_low, uint8_t data) {
    this->commandWrite(0x40);          //line to start writing on (0 -> 64) moves set bits with it DO NOT CHANGE
    this->commandWrite(0xB0 + page);   //writes the page address (4 bits, 8 rows selcted by values 0-7 )
    this->commandWrite(0x10 + col_up); //writes the first 4 bits of the column select (out of 8 bits)
    this->commandWrite(0x00 + col_low);//writes the second 4 bits of the column select (out)

    this->dataWrite(data);//writes 8 vertical bits based on value between 0-255 based on bits set ex: 01001100(0x4C) is |WHITE|
                          //                                                                                                 |BLACK|
                          //                                                                                                 |WHITE|
                          //                                                                                                 |WHITE|
                          //                                                                                                 |BLACK|
                          //                                                                                                 |BLACK|
                          //                                                                                                 |WHITE|
                          //                                                                                                 |WHITE|
}

void LCD::clearLCD(const uint8_t* bitMap) {
    uint8_t page = 0xB0;
    this->commandWrite(0xAE);      //Display OFF
    this->commandWrite(0x40);      //Display start address + 0x40
    for (int i = 0; i < 8; i++) {      //64 pixel display / 8 pixels per page = 8 pages
        this->commandWrite(page);  //send page address
        this->commandWrite(0x10);  //column address upper 4 bits + 0x10
        this->commandWrite(0x00);  //column address lower 4 bits + 0x00
        for (int j = 0; j < 128; j++) { //128 columns wide
            this->dataWrite(0x00); //write clear pixels
            bitMap++;
        }
        page++; //after 128 columns, go to next page
    }
    this->commandWrite(0xAF);
}

void LCD::displayMap(const uint8_t* bitMap) {
    uint8_t page = 0xB0;
    this->commandWrite(0xAE);        //Display OFF
    this->commandWrite(0x40);        //Display start address + 0x40
    for (int i = 0; i < 8; i++) {        //64 pixel display / 8 pixels per page = 8 pages
        this->commandWrite(page);    //send page address
        this->commandWrite(0x10);    //column address upper 4 bits + 0x10
        this->commandWrite(0x00);    //column address lower 4 bits + 0x00
        for (int j = 0; j < 128; j++) {  //128 columns wide
            this->dataWrite(*bitMap); //write clear pixels
            bitMap++;
        }
        page++; //after 128 columns, go to next page
    }
    this->commandWrite(0xAF);
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

}// namespace EVT::core::DEV