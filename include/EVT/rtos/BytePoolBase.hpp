
#ifndef _EVT_RTOS_BYTEPOOL_
#define _EVT_RTOS_BYTEPOOL_

#include <EVT/rtos/Enums.hpp>
#include <cstdint>

namespace core::rtos {

/**
 * A pure virtual class that allows us to pass the BytePool template class into
 * external methods without the compiler complaining.
 */
class BytePoolBase {
public:

    /**
     * Allocates memory from the BytePool and returns a pointer to the start of it.
     *
     * @param[in] amount how much memory (in bytes) that is requested.
     * @param[out] memoryPointer a reference to the pointer to the block of memory that is requested.
     * @param[in] waitOption How long (in ticks) the calling thread should wait for the memory to become available:
     * 0 for no wait, and TX_WAIT_FOREVER for waiting forever.
     * @return The first error found by the function (or Success if there was no error).
     */
    virtual TXError allocateMemory(std::size_t amount, void** memoryPointer, uint32_t waitOption) = 0;

    /**
     * Initializes the bytepool within the threadx kernel.
     */
    virtual TXError init() = 0;
};

} //namespace core::rtos

#endif //_EVT_RTOS_BYTEPOOL_