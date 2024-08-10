#include <core/io/I2C.hpp>

namespace core::IO {

I2C::I2C(Pin sclPin, Pin sdaPin) : sclPin(sclPin), sdaPin(sdaPin) {}

I2C::I2CStatus I2C::write(uint8_t addr, uint8_t* bytes, uint8_t length) {
    for (int i = 0; i < length; i++)
        I2C_RETURN_IF_ERR(write(addr, bytes[i]));
    return I2C::I2CStatus::OK;
}

I2C::I2CStatus I2C::read(uint8_t addr, uint8_t* bytes, uint8_t length) {
    for (int i = 0; i < length; i++)
        I2C_RETURN_IF_ERR(read(addr, &bytes[i]));
    return I2C::I2CStatus::OK;
}

I2C::I2CStatus I2C::writeReg(uint8_t addr, uint8_t reg, uint8_t byte) {

    I2C_RETURN_IF_ERR(write(addr, reg));
    I2C_RETURN_IF_ERR(write(addr, byte));

    return I2C::I2CStatus::OK;
}

/**
 * Reading a register usually involves writing out a byte then reading
 * a byte.
 */
I2C::I2CStatus I2C::readReg(uint8_t addr, uint8_t reg, uint8_t* output) {
    I2C_RETURN_IF_ERR(write(addr, reg));
    return read(addr, output);
}

I2C::I2CStatus I2C::writeReg(uint8_t addr, uint8_t* reg, uint8_t regLength, uint8_t* bytes, uint8_t length) {
    I2C_RETURN_IF_ERR(write(addr, reg, regLength));
    return write(addr, bytes, length);
}

/**
 * Reading from a register with a non-8 byte address and a non-8 byte
 * response usually involves writing out the register address in groups
 * of 8 bytes starting with the MSB. Reading the response is also done,
 * starting with the LSB.
 */
I2C::I2CStatus I2C::readReg(uint8_t addr, uint8_t* reg, uint8_t regLength, uint8_t* bytes, uint8_t length) {
    // Write out register address
    I2C_RETURN_IF_ERR(write(addr, reg, regLength));

    // Read in response
    I2C_RETURN_IF_ERR(read(addr, bytes, length));

    return I2C::I2CStatus::OK;
}

} // namespace core::IO
