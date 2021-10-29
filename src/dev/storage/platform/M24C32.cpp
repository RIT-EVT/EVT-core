#include <EVT/dev/storage/platform/M24C32.hpp>

namespace EVT::core::DEV {

M24C32::M24C32(uint8_t i2cSlaveAddress, IO::I2C &i2c) : i2cSlaveAddress(i2cSlaveAddress), i2c(i2c) {}

uint8_t M24C32::readByte(uint32_t address) {
    uint8_t buffer = 0xff;
    i2c.readMemReg(i2cSlaveAddress, address, &buffer, MEM_ADDRESS_SIZE);
    return buffer;
}

uint16_t M24C32::readHalfWord(uint32_t address) {
    return 0;
}

uint32_t M24C32::readWord(uint32_t address) {
    return 0;
}

uint8_t *M24C32::readBytes(uint8_t *addresses) {
    return nullptr;
}

uint16_t *M24C32::readHalfWords(uint8_t *addresses) {
    return nullptr;
}

uint32_t *M24C32::readWords(uint8_t *addresses) {
    return nullptr;
}

void M24C32::writeByte(uint32_t address, uint8_t data) {
    i2c.writeMemReg(i2cSlaveAddress, address, data, MEM_ADDRESS_SIZE);
}

void M24C32::writeHalfWord(uint32_t address, uint16_t data) {

}

void M24C32::writeWord(uint32_t address, uint32_t data) {

}

void M24C32::writeBytes(uint8_t *addresses, uint8_t *dataArr) {

}

void M24C32::writeHalfWords(uint8_t *addresses, uint16_t *dataArr) {

}

void M24C32::writeWords(uint8_t *addresses, uint32_t *dataArr) {

}
}