#ifndef EVT_EEPROM_HPP
#define EVT_EEPROM_HPP

#include <stdint.h>

namespace EVT::core::DEV {
/**
 * EEPROMs are a type of small non-volatile storage devices. This class
 * represents features common across all EEPROMs.
 */
class EEPROM {

public:
    /**
     * Read 8 bits of data from the given address.
     *
     * @param address[in] The address to read from
     * @return The 8 bits of data at the address
     */
    virtual uint8_t readByte(uint32_t address) = 0;

    /**
     * Read 16 bits of data from the given address.
     *
     * @param address[in] The address to read from
     * @return The 16 bits of data at the address
     */
    virtual uint16_t readHalfWord(uint32_t address) = 0;

    /**
     * Read 32 bits of data from the given address.
     *
     * @param address[in] The address to read from
     * @return The 32 bits of data at the address
     */
    virtual uint32_t readWord(uint32_t address) = 0;

    /**
     * Read a number of consecutive bytes starting at a given memory address.
     *
     * @param address[in] The memory address to start reading from
     * @param buffer[out] Buffer to output bytes to
     * @param numBytes[in] The number of bytes to read
     */
    virtual void readBytes(uint8_t address, uint8_t* buffer, uint8_t numBytes) = 0;

    /**
     * Read a number of consecutive half words starting at a given memory address.
     *
     * @param address[in] The memory address to start reading from
     * @param buffer[out] Buffer to output half words to
     * @param numHWords[in] The number of half words to read
     */
    virtual void readHalfWords(uint8_t address, uint16_t* buffer, uint8_t numHWords) = 0;

    /**
     * Read a number of consecutive words starting at a given memory address.
     *
     * @param address[in] The memory address to start reading from
     * @param buffer[out] Buffer to output words to
     * @param numHWords[in] The number of words to read
     */
    virtual void readWords(uint8_t address, uint32_t* buffer, uint8_t numWords) = 0;

    /**
     * Write 8 bits of data to the given address.
     *
     * @param address[in] The address to write to
     * @param data[in] The data to write out
     */
    virtual void writeByte(uint32_t address, uint8_t data) = 0;

    /**
     * Write 16 bits of data to the given address.
     *
     * @param address[in] The address to write to
     * @param data[in] The data to write out
     */
    virtual void writeHalfWord(uint32_t address, uint16_t data) = 0;

    /**
     * Write 32 bits of data to the given address.
     *
     * @param address[in] The address to write to
     * @param data[in] The data to write out
     */
    virtual void writeWord(uint32_t address, uint32_t data) = 0;

    /**
     * Write a number of consecutive bytes to a given memory address.
     *
     * @param address[in] The address to start writing to
     * @param dataArr[in] The array of bytes to write out
     * @param numBytes[in] The number of bytes to write out
     */
    virtual void writeBytes(uint8_t address, uint8_t* dataArr, uint8_t numBytes) = 0;

    /**
     * Write a number of consecutive half words to a given memory address.
     *
     * @param address[in] The address to start writing to
     * @param dataArr[in] The array of half words to write out
     * @param numHWords[in] The number of half words to write out
     */
    virtual void writeHalfWords(uint8_t address, uint16_t* dataArr, uint8_t numHWords) = 0;

    /**
     * Write a number of consecutive half words to a given memory address.
     *
     * @param address[in] The address to start writing to
     * @param dataArr[in] The array of half words to write out
     * @param numHWords[in] The number of half words to write out
     */
    virtual void writeWords(uint8_t address, uint32_t* dataArr, uint8_t numWords) = 0;
};
}// namespace EVT::core::DEV

#endif//EVT_EEPROM_HPP
