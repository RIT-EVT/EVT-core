//
// Created by matth on 10/2/2021.
//

#include <EVT/io/pin.hpp>
#include <EVT/io/platform/f3xx/f302x8/SPIf302x8.hpp>

namespace EVT::core::IO {

    SPIf302x8::SPIf302x8(GPIO **CSPins, uint8_t pinLength, Pin sckPin, Pin mosiPin, Pin misoPin) :
                        SPI(CSPins,pinLength,sckPin, mosiPin,misoPin) {

    }

    SPIf302x8::SPIf302x8(GPIO **CSPins, uint8_t pinLength, Pin sckPin, Pin mosiPin) :
            SPI(CSPins,pinLength,sckPin, mosiPin) {

    }

    void SPIf302x8::startTransmition(uint8_t device) {

    }

    void SPIf302x8::endTransmition(uint8_t device) {

    }

    void SPIf302x8::write(uint8_t byte) {

    }

    uint8_t SPIf302x8::read() {
        return 0;
    }
}

