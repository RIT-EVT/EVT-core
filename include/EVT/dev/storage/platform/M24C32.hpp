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
     * Read 8 bits of data from each of the given addresses.
     *
     * @param[in] addresses The addresses to read from
     * @return A list of 8 bits of data stored at the given addresses
     */
    uint8_t * readBytes(uint8_t addresses[]);

    /**
     * Read 16 bits of data from each of the given addresses.
     *
     * @param[in] addresses The addresses to read from
     * @return A list of 16 bits of data stored at the given addresses
     */
    uint16_t * readHalfWords(uint8_t addresses[]);

    /**
     * Read 32 bits of data from each of the given addresses.
     *
     * @param[in] addresses The addresses to read from
     * @return A list of 32 bits of data stored at the given addresses
     */
    uint32_t * readWords(uint8_t addresses[]);

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
     * Write 8 bits of data to each of the given addresses.
     *
     * @param addresses The addresses to write to
     * @param[in] dataArr The data to write out
     */
    void writeBytes(uint8_t addresses[], uint8_t dataArr[]);

    /**
     * Write 16 bits of data to each of the given addresses.
     *
     * @param addresses The addresses to write to
     * @param[in] dataArr The data to write out
     */
    void writeHalfWords(uint8_t addresses[], uint16_t dataArr[]);

    /**
     * Write 32 bits of data to each of the given addresses.
     *
     * @param addresses The addresses to write to
     * @param[in] dataArr The data to write out
     */
    void writeWords(uint8_t addresses[], uint32_t dataArr[]);

private:
    /**
     * Size of the memory address of this EEPROM
     * The value is 2 because it has 2-byte addresses
     */
    static constexpr int MEM_ADDRESS_SIZE = 2;
    uint8_t i2cSlaveAddress;
    IO::I2C &i2c;
};
}


#endif //EVT_M24C32
