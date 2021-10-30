#ifndef _EVT_I2Cf302x8_
#define _EVT_I2Cf302x8_

#include <stdint.h>

#include <EVT/io/I2C.hpp>
#include <HALf3/stm32f3xx.h>

namespace EVT::core::IO {

class I2Cf302x8 : public I2C {
public:
    /**
     * Make an instance of an I2C interface for the F3. Will determine
     * which I2C bus of the STM to use based on the provided pins.
     *
     * @param sclPin[in] The I2C clock pin
     * @param sdaPin[in] The I2C data pin
     */
    I2Cf302x8(Pin sclPin, Pin sdaPin);

    /**
     * Write a single byte out over I2C.
     *
     * @param addr[in] The 7 bit unshifted I2C address to write to
     * @param byte[in] The value to write over I2C.
     */
    void write(uint8_t addr, uint8_t byte);

    /**
     * Read a single byte back from the I2C bus.
     *
     * @param addr[in] The 7 bit unshifted I2C address to read from.
     * @return The byte read from the address.
     */
    uint8_t read(uint8_t addr);

    /**
     * Write out multiple bytes over I2C. Each byte will be written one by
     * one.
     *
     * @param addr[in] The 7 bit unshifted I2C address to write to.
     * @param bytes[in] The bytes to write out over I2C.
     * @param length[in] The number of bytes to write out.
     */
    void write(uint8_t addr, uint8_t* bytes, uint8_t length);

    /**
     * Read multiple bytes from an I2C device.
     *
     * @param addr[in] The 7 bit unshifted I2C address to write to
     * @param bytes[out] The buffer to fill with the read in bytes.
     * @param length[in] The number of bytes to read.
     */
    void read(uint8_t addr, uint8_t* bytes, uint8_t length);

    /**
     * Write a value to a register has that 8 bit addresses and 8 bit values.
     *
     * @param addr[in] The 7 bit unshifted I2C address to write to.
     * @param reg[in] The 8 bit register
     * @param byte[in] The byte to write out
     */
    void writeReg(uint8_t addr, uint8_t reg, uint8_t byte);

    /**
     * Read a value from a register that has an 8 bit address and 8 bit value
     *
     * @param addr[in] The 7 bit unshifted I2C address to read from.
     * @param reg[in] The 8 bit register
     * @return The 8 bit value of the register.
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
     * @param addr[in] The 7 bit unshifted I2C address to read from.
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
    void writeMemReg(uint8_t addr, uint32_t memAddress, uint8_t byte, uint16_t memAddSize, uint8_t maxWriteTime);

    /**
     * Read a single byte from a register in memory.
     *
     * @param addr[in] The 7 bit unshifted I2C address to read from
     * @param memAddress[in] The word containing the register to read from
     * @param byte[out] The byte read from memory
     * @param memAddSize[in] The number of bytes in the memory address (1 or 2)
     */
    uint8_t readMemReg(uint8_t addr, uint32_t memAddress, uint8_t* byte, uint16_t memAddSize);

    /**
     * Write a number of bytes to consecutive registers in memory, starting at a specified register.
     *
     * @param addr[in] The 7 bit unshifted I2C address to write to
     * @param memAddress[in] The word containing the register to start writing to
     * @param byte[in] The list of data to write out
     * @param size[in] The number of bytes to be written
     * @param memAddSize[in] The number of bytes in the memory address (1 or 2)
     */
    void writeMemReg(uint8_t addr, uint32_t memAddress, uint8_t* byte, uint8_t size, uint16_t memAddSize, uint8_t maxWriteTime);

    /**
     * Read a number of consecutive bytes, starting at a specified register in memory.
     *
     * @param addr[in] The 7 bit unshifted I2C address to read from
     * @param memAddress[in] The word containing the register to start reading from
     * @param byte[out] The list of bytes read from memory
     * @param size[in] The number of bytes to be read
     * @param memAddSize[in] The number of bytes in the memory address (1 or 2)
     */
    uint8_t readMemReg(uint8_t addr, uint32_t memAddress, uint8_t* byte, uint8_t size, uint16_t memAddSize);

private:
    constexpr static uint32_t DEFAULT_I2C_FREQ = 100000;
    /** Timeout in milliseconds **/
    constexpr static uint32_t DEFAULT_I2C_TIMEOUT = 100;
    /** Interface into the HAL */
    I2C_HandleTypeDef halI2C;
};

}  // namespace EVT::core::IO

#endif
