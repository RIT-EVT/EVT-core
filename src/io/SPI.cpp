#include <EVT/io/SPI.hpp>

namespace EVT::core::IO {
SPI::SPI(GPIO* CSPins[], uint8_t pinLength, Pin sckPin, Pin mosiPin, Pin misoPin) : CSPinsLength(pinLength),
                                                                                    sckPin(sckPin),
                                                                                    mosiPin(mosiPin),
                                                                                    misoPin(misoPin) {
    if (pinLength >= MAX_PINS) {
        CSPinsLength = MAX_PINS;
    }
    for (int i = 0; i < CSPinsLength; i++) {
        this->CSPins[i] = CSPins[i];
    }
}

SPI::SPI(GPIO* CSPins[], uint8_t pinLength, Pin sckPin, Pin mosiPin) : CSPinsLength(pinLength), sckPin(sckPin),
                                                                       mosiPin(mosiPin) {
    if (pinLength >= MAX_PINS) {
        CSPinsLength = MAX_PINS;
    }
    for (int i = 0; i < CSPinsLength; i++) {
        this->CSPins[i] = CSPins[i];
    }
}

void SPI::write(uint8_t* bytes, uint8_t length) {
    for (int i = 0; i < length; i++)
        write(bytes[i]);
}

void SPI::read(uint8_t* bytes, uint8_t length) {
    for (int i = 0; i < length; i++)
        bytes[i] = read();
}

void SPI::writeReg(uint8_t device, uint8_t reg, uint8_t byte) {
    if (startTransmission(device)) {
        write(reg);
        write(byte);
        endTransmission(device);
    }
}

uint8_t SPI::readReg(uint8_t device, uint8_t reg) {
    uint8_t data = 0;
    if (startTransmission(device)) {
        write(reg);
        data = read();
        endTransmission(device);
    }
    return data;
}

void SPI::writeReg(uint8_t device, uint8_t reg, uint8_t* bytes, uint8_t length) {
    if (startTransmission(device)) {
        write(reg);
        write(bytes, length);
        endTransmission(device);
    }
}

void SPI::readReg(uint8_t device, uint8_t reg, uint8_t* bytes, uint8_t length) {
    if (startTransmission(device)) {
        write(reg);
        read(bytes, length);
        endTransmission(device);
    }
}

}// namespace EVT::core::IO
