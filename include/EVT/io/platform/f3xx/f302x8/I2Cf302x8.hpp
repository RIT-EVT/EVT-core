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
     * @param sclPin The I2C clock pin
     * @param sdaPin The I2C data pin
     */
    I2Cf302x8(Pin sclPin, Pin sdaPin);

    /**
     * Write a single byte out over I2C.
     *
     * @param addr The 7 bit unshifted I2C address to write to
     * @param byte The value to write over I2C.
     */
    void write(uint8_t addr, uint8_t byte);

    /**
     * Read a single byte back from the I2C bus.
     *
     * @param addr The 7 bit unshifted I2C address to read from.
     * @return The byte read from the address.
     */
    uint8_t read(uint8_t addr);

    /**
     * Write out multiple bytes over I2C. Each byte will be written one by
     * one.
     *
     * @param addr The 7 bit unshifted I2C address to write to.
     * @param bytes The bytes to write out over I2C.
     * @param length The number of bytes to write out.
     */
    void write(uint8_t addr, uint8_t* bytes, uint8_t length);

    /**
     * Read multiple bytes from an I2C device.
     *
     * @param addr The 7 bit unshifted I2C address to write to
     * @param bytes The buffer to fill with the read in bytes.
     * @param length The number of bytes to read.
     */
    void read(uint8_t addr, uint8_t* bytes, uint8_t length);

    /**
     * Read a value from a register that has an 8 bit address and 8 bit value
     *
     * @param addr The 7 bit unshifted I2C address to read from.
     * @param reg The 8 bit register
     * @return The 8 bit value of the register.
     */
    uint8_t readReg(uint8_t addr, uint8_t reg);

    /**
     * Read a value from a register.
     *
     * @param addr The 7 bit unshifted I2C address to read from.
     * @param reg The bytes containing the register to read from
     * @param regLength The size in bytes of the register
     * @param bytes The bytes read from the register
     * @param length The size of the data returned by the register in bytes
     */
    void readReg(uint8_t addr, uint8_t* reg, uint8_t regLength,
            uint8_t* bytes, uint8_t length);

private:
    constexpr static uint32_t DEFAULT_I2C_FREQ = 100000;
    constexpr static uint32_t DEFAULT_I2C_TIMEOUT = 100;
    /** Interface into the HAL */
    I2C_HandleTypeDef halI2C;
};

}  // namespace EVT::core::IO

#endif
