#ifndef _EVT_Flashf302x8_
#define _EVT_Flashf302x8_

#include <EVT/io/Flash.hpp>

namespace EVT::core::IO {

class Flashf302x8 : public Flash {
public:

    /**
     * Empty constructor, enables writting to STM flash
     */
    Flashf302x8();

    /**
     * Read 8 bits of data from the given address.
     *
     * @param[in] address The address to read from
     * @return The 8 bits of data at the address
     */
    uint8_t readByte(uint32_t address);

    /**
     * Read 16 bites of data from the given address.
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
     * Read the 64 bits of data from the givne address.
     *
     * @param[in] address The address to read from
     * @return The 32 bits of data the address
     */
    uint64_t readDoubleWord(uint32_t address);

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
     * Write 64 bits of data to the given address.
     *
     * @param[in] address The address to write to
     * @param[in] data The data to write out
     */
    void writeDoubleWord(uint32_t address, uint64_t data);

private:
    /**
     * Starting place in memory that writing can take place.
     * This is the address for the start of page 31 in the
     * STM32f302x8 which is the last page. This is set aside
     * for user manipulation.
     */
    static constexpr uint32_t USER_FLASH_START = 0x0800F000;
    /**
     * Numeric representation of the custom configuration we apply
     * to make 31
     */
    static constexpr uint8_t CUSTOM_PAGE_CONFIG = 123;

};

}  // namespace EVT::core::IO

#endif
