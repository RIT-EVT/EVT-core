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
     * @param sclPin The clock pin
     * @param sdaPin The data pin
     */
    I2C(Pin sclPin, Pin sdaPin);

    /**
     * Write a single byte out over I2C.
     *
     * @param addr The 7 bit unshifted I2C address to write to
     * @param byte The value to write over I2C.
     */
    virtual void write(uint8_t addr, uint8_t byte) = 0;

    /**
     * Read a single byte back from the I2C bus.
     *
     * @param addr The 7 bit unshifted I2C address to read from.
     * @return The byte read from the address.
     */
    virtual uint8_t read(uint8_t addr) = 0;

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
     * Write a value to a register has that 8 bit addresses and 8 bit values.
     *
     * @param addr The 7 bit unshifted I2C address to write to.
     * @param reg The 8 bit register
     * @param byte The byte to write out
     */
    void writeReg(uint8_t addr, uint8_t reg, uint8_t byte);

    /**
     * Read a value from a register that has an 8 bit address and 8 bit value
     *
     * @param addr The 7 bit unshifted I2C address to read from.
     * @param reg The 8 bit register
     * @return The 8 bit value of the register.
     */
    uint8_t readReg(uint8_t addr, uint8_t reg);

    /**
     * Write out a multi byte register value.
     *
     * @param addr The 7 bit unshifted I2C address to write to
     * @param reg The register bytes
     * @param regLength The number of bytes in the register address
     * @param bytes The data to write out
     * @param length The number of bytes in the data
     */
    void writeReg(uint8_t addr, uint8_t* reg, uint8_t regLength,
            uint8_t* bytes, uint8_t length);

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
    /** The I2C clock line */
    Pin sclPin;
    /** The I2C data line */
    Pin sdaPin;
};

}  // namespace EVT::core::IO

#endif
