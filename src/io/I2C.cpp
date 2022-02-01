/**
 * Contains generic implmentations for some of the I2C funcitonality.
 * Some hardware has the ability to implement the functionality in this
 * file so specific implementations of the I2C driver may override these
 * methods for a hardware speedup.
 */
#include <EVT/io/I2C.hpp>

namespace EVT::core::IO {

I2C::I2C(Pin sclPin, Pin sdaPin) : sclPin(sclPin), sdaPin(sdaPin) {}

void I2C::write(uint8_t addr, uint8_t* bytes, uint8_t length) {
    for (int i = 0; i < length; i++)
        write(addr, bytes[i]);
}

void I2C::read(uint8_t addr, uint8_t* bytes, uint8_t length) {
    for (int i = 0; i < length; i++)
        bytes[i] = read(addr);
}

void I2C::writeReg(uint8_t addr, uint8_t reg, uint8_t byte) {
    write(addr, reg);
    write(addr, byte);
}

/**
 * Reading a register usually involves writing out a byte then reading
 * a byte.
 */
uint8_t I2C::readReg(uint8_t addr, uint8_t reg) {
    write(addr, reg);
    return read(addr);
}

void I2C::writeReg(uint8_t addr, uint8_t* reg, uint8_t regLength,
                   uint8_t* bytes, uint8_t length) {
    write(addr, reg, regLength);
    write(addr, bytes, length);
}

/**
 * Reading from a register with a non-8 byte address and a non-8 byte
 * response usually involves writing out the register address in groups
 * of 8 bytes starting with the MSB. Reading the response is also done
 * starting with the LSB.
 */
void I2C::readReg(uint8_t addr, uint8_t* reg, uint8_t regLength,
                  uint8_t* bytes, uint8_t length) {
    // Write out register address
    for (int i = 0; i < regLength; i++)
        write(addr, reg[i]);
    // Read in response
    for (int i = 0; i < length; i++)
        bytes[i] = read(addr);
}

}// namespace EVT::core::IO
