#include <EVT/dev/LCD.hpp>

namespace EVT::core::DEV {

LCD::LCD(EVT::core::IO::GPIO& regSelect,EVT::core::IO::GPIO& reset,EVT::core::IO::GPIO& chipSelect,EVT::core::IO::SPI& spi, uint8_t * bitMap)
    : regSelect(regSelect), reset(reset), chipSelect(chipSelect) spi(spi){
    this->regSelect.writePin(EVT::core::IO::GPIO::State::LOW);
    this->reset.writePin(EVT::core::IO::GPIO::State::LOW);
    this->chipSelect.writePin(EVT::core::IO::GPIO::State::LOW);
    this->spi = spi;

}

void LCD::dataWrite(uint8_t data){
    data = (uint8_t)data;
    this->CS.writePin(EVT::core::IO::GPIO::State::LOW);
    this->reg_select.writePin(EVT::core::IO::GPIO::State::HIGH);
    this->spi.startTransmission(0);
    this->spi.write(&d, 1);
    this->spi.endTransmission(0);
    this->CS.writePin(EVT::core::IO::GPIO::State::HIGH);
}
 
void LCD::commWrite(uint8_t data){

    data = (uint8_t)data;
    this->CS.writePin(EVT::core::IO::GPIO::State::LOW);
    this->reg_select.writePin(EVT::core::IO::GPIO::State::LOW);
    this->spi.startTransmission(0);
    this->spi.write(&d, 1);
    this->spi.endTransmission(0);
    this->CS.writePin(EVT::core::IO::GPIO::State::HIGH);
}

void LCD::drivePixel(uint8_t data){

}

void LCD::clearLCD(uint8_t * bitMap){
    unsigned int i,j;
    unsigned char page = 0xB0;
    this->commWrite(0xAE);          //Display OFF
    this->commWrite(0x40);         //Display start address + 0x40
    for(i=0;i<8;i++){       //64 pixel display / 8 pixels per page = 8 pages
        this->commWrite(page);       //send page address
        this->commWrite(0x10);       //column address upper 4 bits + 0x10
        this->commWrite(0x00);       //column address lower 4 bits + 0x00
    for(j=0;j<128;j++){     //128 columns wide
        this->dataWrite(0x00);    //write clear pixels
        bitMap++; 
        }
        page++;         //after 128 columns, go to next page
        }
    this->commWrite(0xAF); 
}

void LCD::displayMap(uint8_t * bitMap){

}

void LCD::initLCD(){

    this->commWrite(Command::ADCSELECT);   // ADC select 
    this->commWrite(Command::DISPLAYOFF);   // Display OFF
    this->commWrite(Command::COMDIRSCAN);   // COM direction scan 
    this->commWrite(Command::LCDBIASET);   // LCD bias set
    this->commWrite(Command::POWERCONTROLSET);   // Power Control set
    this->commWrite(Command::RESRATIOSET);   // Resistor Ratio Set 
    this->commWrite(Command::ELECTRONICVOLCOMMAND);   // Electronic Volume Command (set contrast) Double Btye: 1 of 2
    this->commWrite(Command::ELECTRONICVOLVALUE);   // Electronic Volume value (contrast value) Double Byte: 2 of 2
    this->commWrite(Command::DISPLAYON);   // Display ON

}

}// namespace EVT::core::DEV