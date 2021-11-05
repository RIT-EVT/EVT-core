#include <EVT/dev/storage/platform/M24C32.hpp>

namespace EVT::core::DEV {

M24C32::M24C32(uint8_t i2cSlaveAddress, IO::I2C &i2c) : i2cSlaveAddress(i2cSlaveAddress), i2c(i2c) {}

uint8_t M24C32::readByte(uint32_t address) {
    uint8_t buffer[1];
    i2c.readMemReg(i2cSlaveAddress, address, buffer, MEM_ADDRESS_SIZE);
    return *buffer;
}

uint16_t M24C32::readHalfWord(uint32_t address) {
    uint8_t buffer[2];
    i2c.readMemReg(i2cSlaveAddress, address, buffer, 2, MEM_ADDRESS_SIZE);
    auto* out = reinterpret_cast<uint16_t *>(buffer);
    return *out;
}

uint32_t M24C32::readWord(uint32_t address) {
    uint8_t buffer[4];
    i2c.readMemReg(i2cSlaveAddress, address, buffer, 4, MEM_ADDRESS_SIZE);
    auto* out = reinterpret_cast<uint32_t *>(buffer);
    return *out;
}

void M24C32::readBytes(uint8_t address, uint8_t numBytes, uint8_t* buffer) {
    i2c.readMemReg(i2cSlaveAddress, address, buffer, numBytes, MEM_ADDRESS_SIZE);
}

void M24C32::readHalfWords(uint8_t address, uint8_t numHWords, uint16_t* buffer) {
    uint8_t numBytes = numHWords * 2;
    uint8_t tempBuf[numBytes];
    i2c.readMemReg(i2cSlaveAddress, address, tempBuf, numBytes, MEM_ADDRESS_SIZE);
    auto* convertedBuf = reinterpret_cast<uint16_t *>(tempBuf);
    for (int i = 0; i < numHWords; i++) buffer[i] = convertedBuf[i];
}

void M24C32::readWords(uint8_t address, uint8_t numWords, uint32_t* buffer) {
    uint8_t numBytes = numWords * 4;
    uint8_t tempBuf[numBytes];
    i2c.readMemReg(i2cSlaveAddress, address, tempBuf, numBytes, MEM_ADDRESS_SIZE);
    auto* convertedBuf = reinterpret_cast<uint32_t *>(tempBuf);
    for (int i = 0; i < numWords; i++) buffer[i] = convertedBuf[i];
}

void M24C32::writeByte(uint32_t address, uint8_t data) {
    i2c.writeMemReg(i2cSlaveAddress, address, data, MEM_ADDRESS_SIZE, MAX_WRITE_TIME);
}

void M24C32::writeHalfWord(uint32_t address, uint16_t data) {
    auto* dataArr = reinterpret_cast<uint8_t *>(&data);
    i2c.writeMemReg(i2cSlaveAddress, address, dataArr, 2, MEM_ADDRESS_SIZE, MAX_WRITE_TIME);
}

void M24C32::writeWord(uint32_t address, uint32_t data) {
    auto* dataArr = reinterpret_cast<uint8_t *>(&data);
    i2c.writeMemReg(i2cSlaveAddress, address, dataArr, 4, MEM_ADDRESS_SIZE, MAX_WRITE_TIME);
}

void M24C32::writeBytes(uint8_t address, uint8_t* dataArr, uint8_t numBytes) {
    i2c.writeMemReg(i2cSlaveAddress, address, dataArr, numBytes, MEM_ADDRESS_SIZE, MAX_WRITE_TIME);
}

void M24C32::writeHalfWords(uint8_t address, uint16_t dataArr[], uint8_t numHWords) {
    auto* tempDataArr = reinterpret_cast<uint8_t *>(dataArr);
    i2c.writeMemReg(i2cSlaveAddress, address, tempDataArr, numHWords * 2, MEM_ADDRESS_SIZE, MAX_WRITE_TIME);
}

void M24C32::writeWords(uint8_t address, uint32_t* dataArr, uint8_t numWords) {
    auto* tempDataArr = reinterpret_cast<uint8_t *>(dataArr);
    i2c.writeMemReg(i2cSlaveAddress, address, tempDataArr, numWords * 4, MEM_ADDRESS_SIZE, MAX_WRITE_TIME);
}
}