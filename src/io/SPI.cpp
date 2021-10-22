//
// Created by matth on 10/2/2021.
//

#include <EVT/io/SPI.hpp>

namespace EVT::core::IO {
    void SPI::write(uint8_t device, uint8_t byte) {
        startTransmition(device);
        write(byte);
        endTransmition(device);
    }

    uint8_t SPI::read(uint8_t device) {
        startTransmition(device);
        uint8_t data = read();
        endTransmition(device);
        return data;
    }

    void SPI::write(uint8_t device, uint8_t *bytes, uint8_t length) {
        startTransmition(device);
        for (int i = 0; i < length; i++)
            write(bytes[i]);
        endTransmition(device);
    }

    void SPI::read(uint8_t device, uint8_t *bytes, uint8_t length) {
        startTransmition(device);
        for (int i = 0; i < length; i++)
            bytes[i] = read();
        endTransmition(device);
    }

    void SPI::writeReg(uint8_t device, uint8_t reg, uint8_t byte) {
        startTransmition(device);
        write(reg);
        write(byte);
        endTransmition(device);
    }

    uint8_t SPI::readReg(uint8_t device, uint8_t reg) {
        startTransmition(device);
        write(reg);
        uint8_t data = read();
        endTransmition(device);
        return data;
    }

} // namespace EVT::core::IO
