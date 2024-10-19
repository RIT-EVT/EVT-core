
#ifndef _EVT_RTOS_BYTEPOOL_
#define _EVT_RTOS_BYTEPOOL_

#include <core/rtos/Enums.hpp>
#include <cstdint>
#include <tx_api.h>

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

    /**
     * Releases the memory at the given pointer from the BytePool.
     *
     * @param[in] memoryPointer pointer to the memory to release.
     * @return The first error found by the function (or Success if there was no error).
     */
    virtual TXError releaseMemory(void* memoryPointer) = 0;

    /**
     * Retrieves the number of unallocated bytes left in the BytePool.
     *
     * @param[out] availableBytes a pointer to a place to store the number of available bytes.
     * @return The first error found by the function (or Success if there was no error).
     */
    virtual TXError getAvailableBytes(uint32_t* availableBytes) = 0;

    /**
     * Retrieves the number of fragments the BytePool has been split into.
     *
     * @param[out] fragments a pointer to a place to store the number of fragments.
     * @return The first error found by the function (or Success if there was no error).
     */
    virtual TXError getFragments(uint32_t* fragments) = 0;

    /**
     * Retrieves the name of the first suspended thread.
     *
     * @param[out] name a pointer to a place to store the name pointer.
     * @return The first error found by the function (or Success if there was no error).
     */
    virtual TXError getNameOfFirstSuspendedThread(char** threadName) = 0;

    /**
     * Retrieves the number of threads that are suspended on this BytePool.
     *
     * @param[out] numSuspendedThreads a pointer to a place to store the number of suspended threads.
     * @return The first error found by the function (or Success if there was no error).
     */
    virtual TXError getNumSuspendedThreads(uint32_t* numSuspendedThreads) = 0;

    /**
     * Retrieves the name of this BytePool.
     *
     * @param[out] name a pointer to a place to store the name pointer.
     * @return The first error found by the function (or Success if there was no error).
     */
    virtual TXError getName(char** name) = 0;
};

} // namespace core::rtos

#endif //_EVT_RTOS_BYTEPOOL_