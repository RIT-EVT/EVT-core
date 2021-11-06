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
     * @param i2cSlaveAddress[in] Slave address to use for I2C communication
     * @param i2c[in] I2C instance to communicate over
     */
    M24C32(uint8_t i2cSlaveAddress, IO::I2C &i2c);

    /**
     * Read 8 bits of data from the given address.
     *
     * @param address[in] The address to read from
     * @return The 8 bits of data at the address
     */
    uint8_t readByte(uint32_t address) override;

    /**
     * Read 16 bits of data from the given address.
     *
     * @param address[in] The address to read from
     * @return The 16 bits of data at the address
     */
    uint16_t readHalfWord(uint32_t address) override;

    /**
     * Read 32 bits of data from the given address.
     *
     * @param address[in] The address to read from
     * @return The 32 bits of data at the address
     */
    uint32_t readWord(uint32_t address) override;

    /**
     * Read a number of consecutive bytes starting at a given memory address.
     *
     * @param address[in] The memory address to start reading from
     * @param buffer[out] Buffer to output bytes to
     * @param numBytes[in] The number of bytes to read
     */
    void readBytes(uint8_t address, uint8_t* buffer, uint8_t numBytes) override;

    /**
     * Read a number of consecutive half words starting at a given memory address.
     *
     * @param address[in] The memory address to start reading from
     * @param buffer[out] Buffer to output half words to
     * @param numHWords[in] The number of half words to read
     */
    void readHalfWords(uint8_t address, uint16_t* buffer, uint8_t numHWords) override;

    /**
     * Read a number of consecutive words starting at a given memory address.
     *
     * @param address[in] The memory address to start reading from
     * @param buffer[out] Buffer to output words to
     * @param numHWords[in] The number of words to read
     */
    void readWords(uint8_t address, uint32_t* buffer, uint8_t numWords) override;

    /**
     * Write 8 bits of data to the given address.
     *
     * @param address[in] The address to write to
     * @param data[in] The data to write out
     */
    void writeByte(uint32_t address, uint8_t data) override;

    /**
     * Write 16 bits of data to the given address.
     *
     * @param address[in] The address to write to
     * @param data[in] The data to write out
     */
    void writeHalfWord(uint32_t address, uint16_t data) override;

    /**
     * Write 32 bits of data to the given address.
     *
     * @param address[in] The address to write to
     * @param data[in] The data to write out
     */
    void writeWord(uint32_t address, uint32_t data) override;

    /**
     * Write a number of consecutive bytes to a given memory address.
     *
     * @param address[in] The address to start writing to
     * @param dataArr[in] The array of bytes to write out
     * @param numBytes[in] The number of bytes to write out
     */
    void writeBytes(uint8_t address, uint8_t* dataArr, uint8_t numBytes) override;

    /**
     * Write a number of consecutive half words to a given memory address.
     *
     * @param address[in] The address to start writing to
     * @param dataArr[in] The array of half words to write out
     * @param numHWords[in] The number of half words to write out
     */
    void writeHalfWords(uint8_t address, uint16_t* dataArr, uint8_t numHWords) override;

    /**
     * Write a number of consecutive words to a given memory address.
     *
     * @param address[in] The address to start writing to
     * @param dataArr[in] The array of words to write out
     * @param numWords[in] The number of words to write out
     */
    void writeWords(uint8_t address, uint32_t* dataArr, uint8_t numWords) override;

private:
    /**
     * Size of the memory address of this EEPROM
     * The value is 2 because it has 2-byte addresses
     */
    static constexpr uint8_t MEM_ADDRESS_SIZE = 2;
    /** Maximum time to write a single byte of this EEPROM */
    static constexpr uint8_t MAX_WRITE_TIME = 5;
    /** Slave address for this device to be used for I2C communication */
    uint8_t i2cSlaveAddress;
    /** I2C instance to be used for I2C communication */
    IO::I2C &i2c;
};
}


#endif //EVT_M24C32
