#ifndef _EVT_I2C_
#define _EVT_I2C_

#include <cstdint>

#define I2C_RETURN_IF_ERR(func)             \
    {                                       \
        I2C::I2CStatus status = func;       \
        if (status != I2C::I2CStatus::OK) { \
            return status;                  \
        }                                   \
    }                                       \
    void(0)

namespace core::IO {

// Forward declarations:
// The different pins are hardware specific. Forward declaration to allow
// at compilation time the decision of which pins should be used.
enum class Pin;

/**
 * Contains generic implementations for some of the I2C functionality.
 * Some hardware has the ability to implement the functionality in this
 * file so specific implementations of the I2C driver may override these
 * methods for a hardware speedup.
 */
class I2C {
public:
    /**
     * Represents potential errors that may take place when using the I2C
     * interface. Each method that interfaces over I2C could potentially
     * return one of these errors, or OK if no error.
     */
    enum class I2CStatus {
        TIMEOUT = 0,
        BUSY = 1,
        ERROR = 2,
        OK = 3
    };

    /**
     * Make an instance of an I2C interface that will use the given pins
     * for clock and data lines.
     *
     * @param[in] sclPin The clock pin
     * @param[in] sdaPin The data pin
     */
    I2C(Pin sclPin, Pin sdaPin);

    /**
     * Write a single byte out over I2C.
     *
     * @param[in] addr The 7 bit unshifted I2C address to write to
     * @param[in] byte The value to write over I2C
     * @return The status of making the write request
     */
    virtual I2CStatus write(uint8_t addr, uint8_t byte) = 0;

    /**
     * Read a single byte back from the I2C bus.
     *
     * @param[in] addr The 7 bit unshifted I2C address to read from
     * @param[out] output The location to store the result from the read
     * @return The status of making the read request
     */
    virtual I2CStatus read(uint8_t addr, uint8_t* output) = 0;

    /**
     * Write out multiple bytes over I2C. Each byte will be written one by
     * one.
     *
     * @param[in] addr The 7 bit unshifted I2C address to write to
     * @param[in] bytes The bytes to write out over I2C
     * @param[in] length The number of bytes to write out
     * @return The status of making the write request
     */
    virtual I2CStatus write(uint8_t addr, uint8_t* bytes, uint8_t length);

    /**
     * Read multiple bytes from an I2C device.
     *
     * @param[in] addr The 7 bit unshifted I2C address to write to
     * @param[out] bytes The buffer to fill with the read in bytes
     * @param[in] length The number of bytes to read
     * @return The status of making the read request
     */
    virtual I2CStatus read(uint8_t addr, uint8_t* bytes, uint8_t length);

    /**
     * Write a value to a register has that 8 bit addresses and 8 bit values.
     *
     * @param[in] addr The 7 bit unshifted I2C address to write to
     * @param[in] reg The 8 bit register
     * @param[in] byte The byte to write out
     * @return The status of attempting to write out to a register
     */
    virtual I2CStatus writeReg(uint8_t addr, uint8_t reg, uint8_t byte);

    /**
     * Read a value from a register that has an 8 bit address and 8 bit value.
     *
     * @param[in] addr The 7 bit unshifted I2C address to read from
     * @param[in] reg The 8 bit register
     * @param[out] output Will store the value of the read request
     * @return The 8 bit value of the register
     */
    virtual I2CStatus readReg(uint8_t addr, uint8_t reg, uint8_t* output);

    /**
     * Write out a multi byte register value.
     *
     * @param[in] addr The 7 bit unshifted I2C address to write to
     * @param[in] reg The register bytes
     * @param[in] regLength The number of bytes in the register address
     * @param[in] bytes The data to write out
     * @param[in] length The number of bytes in the data
     * @return The status of attempting to write out to the register
     */
    virtual I2CStatus writeReg(uint8_t addr, uint8_t* reg, uint8_t regLength,
                               uint8_t* bytes, uint8_t length);

    /**
     * Read a value from a register.
     *
     * @param[in] addr The 7 bit unshifted I2C address to read from
     * @param[in] reg The bytes containing the register to read from
     * @param[in] regLength The size in bytes of the register
     * @param[out] bytes The bytes read from the register
     * @param[in] length The size of the data returned by the register in bytes
     * @return The status of reading from the register
     */
    virtual I2CStatus readReg(uint8_t addr, uint8_t* reg, uint8_t regLength,
                              uint8_t* bytes, uint8_t length);

    /**
     * Write a single byte to a register in memory.
     * This is a separate method from normal I2C communication because memory read/write methods use
     * a slightly different I2C pattern.
     *
     * @param[in] addr The 7 bit unshifted I2C address to write to
     * @param[in] memAddress The word containing the register to write to
     * @param[in] byte The data to write out
     * @param[in] memAddSize The number of bytes in the memory address (1 or 2)
     * @return The status of writing out a memory register
     */
    virtual I2CStatus writeMemReg(uint8_t addr, uint32_t memAddress,
                                  uint8_t byte, uint16_t memAddSize,
                                  uint8_t maxWriteTime) = 0;

    /**
     * Read a single byte from a register in memory.
     * This is a separate method from normal I2C communication because
     * memory read/write methods use
     * a slightly different I2C pattern.
     *
     * @param[in] addr The 7 bit unshifted I2C address to read from
     * @param[in] memAddress The word containing the register to read from
     * @param[out] byte The byte read from memory
     * @param[in] memAddSize The number of bytes in the memory address (1 or 2)
     * @return The result of attempting to read from memory
     */
    virtual I2CStatus readMemReg(uint8_t addr, uint32_t memAddress,
                                 uint8_t* byte, uint16_t memAddSize) = 0;

    /**
     * Write a number of bytes to consecutive registers in memory, starting at a specified register.
     * This is a separate method from normal I2C communication because memory read/write methods use
     * a slightly different I2C pattern.
     *
     * @param[in] addr The 7 bit unshifted I2C address to write to
     * @param[in] memAddress The word containing the register to start writing to
     * @param[in] byte The list of data to write out
     * @param[in] size The number of bytes to be written
     * @param[in] memAddSize The number of bytes in the memory address (1 or 2)
     * @return The status of writing out to the memory address
     */
    virtual I2CStatus writeMemReg(uint8_t addr, uint32_t memAddress,
                                  uint8_t* byte, uint8_t size,
                                  uint16_t memAddSize,
                                  uint8_t maxWriteTime) = 0;

    /**
     * Read a number of consecutive bytes, starting at a specified register in memory.
     * This is a separate method from normal I2C communication because memory read/write methods use
     * a slightly different I2C pattern.
     *
     * @param[in] addr The 7 bit unshifted I2C address to read from
     * @param[in] memAddress The word containing the register to start reading from
     * @param[out] byte The list of bytes read from memory
     * @param[in] size The number of bytes to be read
     * @param[in] memAddSize The number of bytes in the memory address (1 or 2)
     * @param[out] output The value to store the read back memory
     * @return The status of reading from the memory address;
     */
    virtual I2CStatus readMemReg(uint8_t addr, uint32_t memAddress,
                                 uint8_t* byte, uint8_t size,
                                 uint16_t memAddSize) = 0;

private:
    /** The I2C clock line */
    Pin sclPin;
    /** The I2C data line */
    Pin sdaPin;
};

}// namespace core::IO

#endif
