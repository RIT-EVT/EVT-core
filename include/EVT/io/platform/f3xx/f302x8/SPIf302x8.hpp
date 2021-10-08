#ifndef _EVT_SPIf302x8_
#define _EVT_SPIf302x8_

#include <stdint.h>

#include <EVT/io/SPI.hpp>
#include <HALf3/stm32f3xx.h>

namespace EVT::core::IO {
    class SPIf302x8: public SPI{
        SPIf302x8(GPIO* CSPins[], uint8_t pinLength, Pin sckPin, Pin mosiPin, Pin misoPin);
        SPIf302x8(GPIO* CSPins[], uint8_t pinLength, Pin sckPin, Pin mosiPin);

        void startTransmition(uint8_t device);
        void endTransmition(uint8_t device);
        void write(uint8_t byte);
        uint8_t read();
    };
}

#endif