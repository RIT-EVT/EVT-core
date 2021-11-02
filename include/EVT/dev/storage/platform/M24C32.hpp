#ifndef EVT_M24C32
#define EVT_M24C32

#include <EVT/dev/storage/EEPROM.hpp>
#include <EVT/io/I2C.hpp>

namespace EVT::core::DEV {


class M24C32 : private EEPROM {
public:

    /**
     * Start up the EEPROM with a slave address of i2cSlaveAddress and
     * I2C instance i2c
     *
     * @param i2cSlaveAddress Slave address to use for I2C communication
     * @param i2c I2C instance to communicate over
     */
    M24C32(uint8_t i2cSlaveAddress, IO::I2C &i2c);

    /**
     * Read 8 bits of data from the given address.
     *
     * @param[in] address The address to read from
     * @return The 8 bits of data at the address
     */
    uint8_t readByte(uint32_t address);

    /**
     * Read 16 bits of data from the given address.
     *
     * @param[in] address The address to read from
     * @return The 16 bits of data at the address
     */
    uint16_t readHalfWord(uint32_t address);

    /**
     * Read 32 bits of data from the given address.
     *
     * @param[in] address The address to read from
     * @return The 32 bits of data at the address
     */
    uint32_t readWord(uint32_t address);

    /**
     * Read a number of consecutive bytes starting at a given memory address.
     *
     * @param[in] address The memory address to start reading from
     * @param[in] numBytes The number of bytes to read
     * @param[out] buffer Buffer to output bytes to
     */
    void readBytes(uint8_t address, uint8_t numBytes, uint8_t* buffer);

    /**
     * Read a number of consecutive half words starting at a given memory address.
     *
     * @param[in] address The memory address to start reading from
     * @param[in] numHWords The number of half words to read
     * @param[out] buffer Buffer to output half words to
     */
    void readHalfWords(uint8_t address, uint8_t numHWords, uint16_t* buffer);

    /**
     * Read a number of consecutive words starting at a given memory address.
     *
     * @param[in] address The memory address to start reading from
     * @param[in] numHWords The number of words to read
     * @param[out] buffer Buffer to output words to
     */
    void readWords(uint8_t address, uint8_t numWords, uint32_t* buffer);

    /**
     * Write 8 bits of data to the given address.
     *
     * @param[in] address The address to write to
     * @param[in] data The data to write out
     */
    void writeByte(uint32_t address, uint8_t data);

    /**
     * Write 16 bits of data to the given address.
     *
     * @param[in] address The address to write to
     * @param[in] data The data to write out
     */
    void writeHalfWord(uint32_t address, uint16_t data);

    /**
     * Write 32 bits of data to the given address.
     *
     * @param[in] address The address to write to
     * @param[in] data The data to write out
     */
    void writeWord(uint32_t address, uint32_t data);

    /**
     * Write a number of consecutive bytes to a given memory address.
     *
     * @param address The address to start writing to
     * @param[in] dataArr The array of bytes to write out
     */
    void writeBytes(uint8_t address, uint8_t* dataArr);

    /**
     * Write a number of consecutive half words to a given memory address.
     *
     * @param address The address to start writing to
     * @param[in] dataArr The array of half words to write out
     */
    void writeHalfWords(uint8_t address, uint16_t* dataArr);

    /**
     * Write a number of consecutive words to a given memory address.
     *
     * @param address The address to start writing to
     * @param[in] dataArr The array of words to write out
     */
    void writeWords(uint8_t address, uint32_t* dataArr);

private:
    /**
     * Size of the memory address of this EEPROM
     * The value is 2 because it has 2-byte addresses
     */
    static constexpr uint8_t MEM_ADDRESS_SIZE = 2;
    /**
     * Maximum time to write a single byte of this EEPROM
     */
    static constexpr uint8_t MAX_WRITE_TIME = 5;
    uint8_t i2cSlaveAddress;
    IO::I2C &i2c;
};
}


#endif //EVT_M24C32
