#include <EVT/dev/LCD.hpp>

namespace EVT::core::DEV {

LCD::LCD(EVT::core::IO::GPIO& regSelect,EVT::core::IO::GPIO& reset,EVT::core::IO::GPIO& chipSelect,EVT::core::IO::SPI& spi, uint8_t * bitMap){

}

void LCD::dataWrite(uint8_t data){

}
 
void LCD::commWrite(LCD::Command command){

}

void LCD::drivePixel(uint8_t data){

}

void LCD::clearLCD(uint8_t * bitMap){

}

void LCD::displayMap(uint8_t * bitMap){

}

void LCD::initLCD(){

}

}// namespace EVT::core::DEV