#include <EVT/dev/LCD.hpp>

namespace EVT::core::DEV {

LCD::LCD(EVT::core::IO::GPIO& regSelect,EVT::core::IO::GPIO& reset,EVT::core::IO::GPIO& chipSelect,EVT::core::IO::SPI& spi, unsigned char * bitMap)
    : regSelect(regSelect), reset(reset), chipSelect(chipSelect) spi(spi){
    this->regSelect.writePin(EVT::core::IO::GPIO::State::LOW);
    this->reset.writePin(EVT::core::IO::GPIO::State::LOW);
    this->chipSelect.writePin(EVT::core::IO::GPIO::State::LOW);
    this->spi = spi;

}

void LCD::dataWrite(unsigned char data){
    data = (uint8_t)data;
    this->CS.writePin(EVT::core::IO::GPIO::State::LOW);
    this->reg_select.writePin(EVT::core::IO::GPIO::State::HIGH);
    this->spi.startTransmission(0);
    this->spi.write(&data, 1);
    this->spi.endTransmission(0);
    this->CS.writePin(EVT::core::IO::GPIO::State::HIGH);
}
 
void LCD::commWrite(unsigned char data){

    data = (uint8_t)data;
    this->CS.writePin(EVT::core::IO::GPIO::State::LOW);
    this->reg_select.writePin(EVT::core::IO::GPIO::State::LOW);
    this->spi.startTransmission(0);
    this->spi.write(&data, 1);
    this->spi.endTransmission(0);
    this->CS.writePin(EVT::core::IO::GPIO::State::HIGH);
}

void LCD::drivePixel(unsigned char page, unsigned char col_up, unsigned char col_low, unsigned char data){
    this->comm_write(0x40); //line to start writing on (0 -> 64) moves set bits with it DO NOT CHANGE 
    this->comm_write(0xB0+ page); //writes the page address (4 bits, 8 rows selcted by values 0-7 ) 
    this->comm_write(0x10 + col_up); //writes the first 4 bits of the column select (out of 8 bits)
    this->comm_write(0x00 + col_low); //writes the second 4 bits of the column select (out)

    this->data_write(data); //writes 8 vertical bits based on value between 0-255 based on bits set ex: 01001100 is       |WHITE|
                      //                                                                                            |BLACK|
                      //                                                                                            |WHITE|
                      //                                                                                            |WHITE|
                      //                                                                                            |BLACK|
                      //                                                                                            |BLACK|
                      //                                                                                            |WHITE|
                      //                                                                                            |WHITE|             
}

void LCD::clearLCD(unsigned char * bitMap){
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

void LCD::displayMap(unsigned char * bitMap){

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