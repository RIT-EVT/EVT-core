#ifndef EVT_FMC_HPP
#define EVT_FMC_HPP

#include <cstdint>

namespace core::io {

class FMC {
public:
    /**
     * Initializes an FMC device
     */
    FMC(uint32_t sdramMemoryAddress);

    /**
     * Write a value to SDRAM at the specified byte offset.
     *
     * @param[in] offset Byte offset from the SDRAM base address
     * @param[in] value Value to write to memory
     */
    void write32(uint32_t offset, uint32_t value) const;

    /**
     * Read a value SDRAM at the specified byte offset.
     *
     * @param[in] offset Byte offset from the SDRAM base address
     */
    uint32_t read32(uint32_t offset) const;

protected:
    uint32_t sdramMemoryAddress;
};

} // namespace core::io

#endif // EVT_FMC_HPP
