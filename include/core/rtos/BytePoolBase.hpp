#ifndef _EVT_RTOS_BYTEPOOL_
#define _EVT_RTOS_BYTEPOOL_

#include <core/rtos/Enums.hpp>
#include <cstdint>
#include <tx_api.h>

#ifndef BYTEPOOL_NAME_MAX_LENGTH
    #define BYTEPOOL_NAME_MAX_LENGTH 26
#endif //BYTEPOOL_NAME_MAX_LENGTH


namespace core::rtos {

/**
 * A non-templated base class that allows us to pass the BytePool template class into
 * external methods without the compiler complaining.
 */
class BytePoolBase {
public:
    /**
     * Allocate memory from the BytePool and returns a pointer to the start of it
     *
     * @param[in] amount How much memory (in bytes) that is requested
     * @param[in] waitOption How long (in ticks) the calling thread should wait for the memory to become available:
     * 0 for no wait, and TX_WAIT_FOREVER for waiting forever
     * @param[out] memoryPointer The allocated block of memory
     * @return The first error found by the function or TXE_SUCCESS if there was no error
     */
    virtual TXError allocateMemory(std::size_t amount, uint32_t waitOption, void** memoryPointer) = 0;

    /**
     * Initializes the bytepool within the threadx kernel.
     */
    virtual TXError init() = 0;

    /**
     * Release the memory at the given pointer from the BytePool
     *
     * @param[in] memoryPointer A pointer to the memory to release
     * @return The first error found by the function or TXE_SUCCESS if there was no error
     */
    virtual TXError releaseMemory(void* memoryPointer) = 0;

    /**
     * Get the number of unallocated bytes left in the BytePool
     *
     * @param[out] availableBytes The returned number of available bytes
     * @return The first error found by the function or TXE_SUCCESS if there was no error
     */
    virtual TXError getAvailableBytes(uint32_t* availableBytes) = 0;

    /**
     * Get the number of fragments the BytePool has been split into
     *
     * @param[out] fragments The returned number of fragments
     * @return The first error found by the function or TXE_SUCCESS if there was no error
     */
    virtual TXError getFragments(uint32_t* fragments) = 0;

    /**
     * Get the name of the first suspended thread
     *
     * @param[out] name The returned name
     * @return The first error found by the function or TXE_SUCCESS if there was no error
     */
    virtual TXError getNameOfFirstSuspendedThread(char** threadName) = 0;

    /**
     * Get the number of threads that are suspended on this BytePool
     *
     * @param[out] numSuspendedThreads The returned number of suspended threads
     * @return The first error found by the function or TXE_SUCCESS if there was no error
     */
    virtual TXError getNumSuspendedThreads(uint32_t* numSuspendedThreads) = 0;

    /**
     * Copy the name of this BytePool object into the character array pointed to by destination &
     * insert a null-terminating character at the end of the given array for safety
     *
     * @param[out] destination Character array to copy the name into.
     * Should be BYTEPOOL_NAME_MAX_LENGTH bytes long
     * @param[in] size the size of the output array
     */
    virtual void getName(char* name, size_t size) = 0;
};

} // namespace core::rtos

#endif //_EVT_RTOS_BYTEPOOL_