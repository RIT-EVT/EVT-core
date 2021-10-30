#ifndef _EVT_I2C_
#define _EVT_I2C_

#include <stdint.h>

namespace EVT::core::IO {
// Forward declarations:
// The different pins are hardware specific. Forward declaration to allow
// at compilation time the decision of which pins should be used.
enum class Pin;

class I2C {
public:
    /**
     * Make an instance of an I2C interface that will use the given pins
     * for clock and data lines.
     *
     * @param sclPin[in] The clock pin
     * @param sdaPin[in] The data pin
     */
    I2C(Pin sclPin, Pin sdaPin);

    /**
     * Write a single byte out over I2C.
     *
     * @param addr[in] The 7 bit unshifted I2C address to write to
     * @param byte[in] The value to write over I2C
     */
    virtual void write(uint8_t addr, uint8_t byte) = 0;

    /**
     * Read a single byte back from the I2C bus.
     *
     * @param addr[in] The 7 bit unshifted I2C address to read from
     * @return The byte read from the address
     */
    virtual uint8_t read(uint8_t addr) = 0;

    /**
     * Write out multiple bytes over I2C. Each byte will be written one by
     * one.
     *
     * @param addr[in] The 7 bit unshifted I2C address to write to
     * @param bytes[in] The bytes to write out over I2C
     * @param length[in] The number of bytes to write out
     */
    void write(uint8_t addr, uint8_t* bytes, uint8_t length);

    /**
     * Read multiple bytes from an I2C device.
     *
     * @param addr[in] The 7 bit unshifted I2C address to write to
     * @param bytes[out] The buffer to fill with the read in bytes
     * @param length[in] The number of bytes to read
     */
    void read(uint8_t addr, uint8_t* bytes, uint8_t length);

    /**
     * Write a value to a register has that 8 bit addresses and 8 bit values.
     *
     * @param addr The 7 bit unshifted I2C address to write to
     * @param reg The 8 bit register
     * @param byte The byte to write out
     */
    void writeReg(uint8_t addr, uint8_t reg, uint8_t byte);

    /**
     * Read a value from a register that has an 8 bit address and 8 bit value.
     *
     * @param addr[in] The 7 bit unshifted I2C address to read from
     * @param reg[in] The 8 bit register
     * @return The 8 bit value of the register
     */
    uint8_t readReg(uint8_t addr, uint8_t reg);

    /**
     * Write out a multi byte register value.
     *
     * @param addr[in] The 7 bit unshifted I2C address to write to
     * @param reg[in] The register bytes
     * @param regLength[in] The number of bytes in the register address
     * @param bytes[in] The data to write out
     * @param length[in] The number of bytes in the data
     */
    void writeReg(uint8_t addr, uint8_t* reg, uint8_t regLength,
            uint8_t* bytes, uint8_t length);

    /**
     * Read a value from a register.
     *
     * @param addr[in] The 7 bit unshifted I2C address to read from
     * @param reg[in] The bytes containing the register to read from
     * @param regLength[in] The size in bytes of the register
     * @param bytes[out] The bytes read from the register
     * @param length[in] The size of the data returned by the register in bytes
     */
    void readReg(uint8_t addr, uint8_t* reg, uint8_t regLength,
            uint8_t* bytes, uint8_t length);

    /**
     * Write a single byte to a register in memory.
     *
     * @param addr[in] The 7 bit unshifted I2C address to write to
     * @param memAddress[in] The word containing the register to write to
     * @param byte[in] The data to write out
     * @param memAddSize[in] The number of bytes in the memory address (1 or 2)
     */
    virtual void writeMemReg(uint8_t addr, uint32_t memAddress, uint8_t byte, uint16_t memAddSize, uint8_t maxWriteTime) = 0;

    /**
     * Read a single byte from a register in memory.
     *
     * @param addr[in] The 7 bit unshifted I2C address to read from
     * @param memAddress[in] The word containing the register to read from
     * @param byte[out] The byte read from memory
     * @param memAddSize[in] The number of bytes in the memory address (1 or 2)
     */
    virtual uint8_t readMemReg(uint8_t addr, uint32_t memAddress, uint8_t* byte, uint16_t memAddSize) = 0;

    /**
     * Write a number of bytes to consecutive registers in memory, starting at a specified register.
     *
     * @param addr[in] The 7 bit unshifted I2C address to write to
     * @param memAddress[in] The word containing the register to start writing to
     * @param byte[in] The list of data to write out
     * @param size[in] The number of bytes to be written
     * @param memAddSize[in] The number of bytes in the memory address (1 or 2)
     */
    virtual void writeMemReg(uint8_t addr, uint32_t memAddress, uint8_t* byte, uint8_t size, uint16_t memAddSize, uint8_t maxWriteTime) = 0;

    /**
     * Read a number of consecutive bytes, starting at a specified register in memory.
     *
     * @param addr[in] The 7 bit unshifted I2C address to read from
     * @param memAddress[in] The word containing the register to start reading from
     * @param byte[out] The list of bytes read from memory
     * @param size[in] The number of bytes to be read
     * @param memAddSize[in] The number of bytes in the memory address (1 or 2)
     */
    virtual uint8_t readMemReg(uint8_t addr, uint32_t memAddress, uint8_t* byte, uint8_t size, uint16_t memAddSize) = 0;

private:
    /** The I2C clock line */
    Pin sclPin;
    /** The I2C data line */
    Pin sdaPin;
};

}  // namespace EVT::core::IO

#endif
