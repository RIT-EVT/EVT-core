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
    uint8_t readByte(size_t address);

    /**
     * Read 16 bites of data from the given address.
     *
     * @param[in] address The address to read from
     * @return The 16 bits of data at the address
     */
    uint16_t readHalfWord(size_t address);

    /**
     * Read 32 bits of data from the given address.
     *
     * @param[in] address The address to read from
     * @return The 32 bits of data at the address
     */
    uint32_t readWord(size_t address);

    /**
     * Read the 64 bits of data from the givne address.
     *
     * @param[in] address The address to read from
     * @return The 32 bits of data the address
     */
    uint64_t readDoubleWord(size_t address);

    /**
     * Write 8 bits of data to the given address.
     *
     * @param[in] address The address to write to
     * @param[in] data The data to write out
     */
    void writeByte(size_t address, uint8_t data);

    /**
     * Write 16 bits of data to the given address.
     *
     * @param[in] address The address to write to
     * @param[in] data The data to write out
     */
    void writeHalfWord(size_t address, uint16_t data);

    /**
     * Write 32 bits of data to the given address.
     *
     * @param[in] address The address to write to
     * @param[in] data The data to write out
     */
    void writeWord(size_t address, uint32_t data);

    /**
     * Write 64 bits of data to the given address.
     *
     * @param[in] address The address to write to
     * @param[in] data The data to write out
     */
    void writeDoubleWord(size_t address, uint64_t data);
};
}  // namespace EVT::core::IO
#endif
