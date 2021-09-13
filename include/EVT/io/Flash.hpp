#ifndef _EVT_FLASH_
#define _EVT_FLASH_

#include <stdint.h>

namespace EVT::core::IO {

class Flash {

public:

    /**
     * Read 8 bits of data from the given address.
     *
     * @param[in] address The address to read from
     * @return The 8 bits of data at the address
     */
    virtual uint8_t readByte(uint32_t address) = 0;

    /**
     * Read 16 bites of data from the given address.
     *
     * @param[in] address The address to read from
     * @return The 16 bits of data at the address
     */
    virtual uint16_t readHalfWord(uint32_t address) = 0;

    /**
     * Read 32 bits of data from the given address.
     *
     * @param[in] address The address to read from
     * @return The 32 bits of data at the address
     */
    virtual uint32_t readWord(uint32_t address) = 0;

    /**
     * Read the 64 bits of data from the givne address.
     *
     * @param[in] address The address to read from
     * @return The 32 bits of data the address
     */
    virtual uint64_t readDoubleWord(uint32_t address) = 0;

    /**
     * Write 8 bits of data to the given address.
     *
     * @param[in] address The address to write to
     * @param[in] data The data to write out
     */
    virtual void writeByte(uint32_t address, uint8_t data) = 0;

    /**
     * Write 16 bits of data to the given address.
     *
     * @param[in] address The address to write to
     * @param[in] data The data to write out
     */
    virtual void writeHalfWord(uint32_t address, uint16_t data) = 0;

    /**
     * Write 32 bits of data to the given address.
     *
     * @param[in] address The address to write to
     * @param[in] data The data to write out
     */
    virtual void writeWord(uint32_t address, uint32_t data) = 0;

    /**
     * Write 64 bits of data to the given address.
     *
     * @param[in] address The address to write to
     * @param[in] data The data to write out
     */
    virtual void writeDoubleWord(uint32_t address, uint64_t data) = 0;
};
}  // namespace EVT::core::IO
#endif
