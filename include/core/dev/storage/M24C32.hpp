#ifndef EVT_M24C32
#define EVT_M24C32

#include <core/dev/storage/EEPROM.hpp>
#include <core/io/I2C.hpp>

namespace core::DEV {

class M24C32 : public EEPROM {
public:
    /**
     * Start up the EEPROM with a slave address of i2cSlaveAddress and
     * I2C instance i2c
     *
     * @param[in] i2cSlaveAddress Slave address to use for I2C communication
     * @param[in] i2c I2C instance to communicate over
     */
    M24C32(uint8_t i2cSlaveAddress, IO::I2C& i2c);

    uint8_t readByte(uint32_t address) override;

    uint16_t readHalfWord(uint32_t address) override;

    uint32_t readWord(uint32_t address) override;

    void readBytes(uint32_t address, uint8_t* buffer, uint8_t numBytes) override;

    void readHalfWords(uint8_t address, uint16_t* buffer, uint8_t numHWords) override;

    void readWords(uint8_t address, uint32_t* buffer, uint8_t numWords) override;

    void writeByte(uint32_t address, uint8_t data) override;

    void writeHalfWord(uint32_t address, uint16_t data) override;

    void writeWord(uint32_t address, uint32_t data) override;

    void writeBytes(uint32_t address, uint8_t* dataArr, uint8_t numBytes) override;

    void writeHalfWords(uint8_t address, uint16_t* dataArr, uint8_t numHWords) override;

    void writeWords(uint8_t address, uint32_t* dataArr, uint8_t numWords) override;

private:
    /**
     * Size of the memory address of this EEPROM
     * The value is 2 because it has 2-byte addresses
     */
    static constexpr uint8_t MEM_ADDRESS_SIZE = 2;
    /** Maximum time to write a single byte of this EEPROM */
    static constexpr uint8_t MAX_WRITE_TIME = 5;
    /** Number of bytes that make up a page size */
    static constexpr uint8_t PAGE_SIZE = 32;
    /** Slave address for this device to be used for I2C communication */
    uint8_t i2cSlaveAddress;
    /** I2C instance to be used for I2C communication */
    IO::I2C& i2c;
};

} // namespace core::DEV

#endif // EVT_M24C32
