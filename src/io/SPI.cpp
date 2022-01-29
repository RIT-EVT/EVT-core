//
// Created by matth on 10/2/2021.
//

#include <EVT/io/SPI.hpp>

namespace EVT::core::IO {
SPI::SPI(GPIO** CSPins, uint8_t pinLength, Pin sckPin, Pin mosiPin, Pin misoPin) : CSPins(*CSPins), CSPinsLength(pinLength), sckPin(sckPin), mosiPin(mosiPin), misoPin(misoPin) {}

SPI::SPI(GPIO** CSPins, uint8_t pinLength, Pin sckPin, Pin mosiPin) : CSPins(*CSPins), CSPinsLength(pinLength), sckPin(sckPin), mosiPin(mosiPin) {}

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
