#ifndef _EVT_RTOS_BYTEPOOLBASE_
#define _EVT_RTOS_BYTEPOOLBASE_

#include <core/rtos/BytePoolBase.hpp>
#include <cstdint>

namespace core::rtos {

/**
 * Template class that wraps a ThreadX BytePool. The BytePool can be used essentially as a heap
 * within a static block of memory via the allocateMemory() and releaseMemory() methods.
 * @tparam SIZE How large the bytepool is.
 */
template<std::size_t SIZE>
class BytePool : public BytePoolBase {
public:
    /**
     * Construct a BytePool, including creating a buffer to hold the
     * information for the pool and the buffer for the pool itself
     *
     * @param[in] name String name of the BytePool
     */
    BytePool(char* name) : name(name), buffer(), txBytePool() {}

    /**
     * BytePool deconstructor
     */
    ~BytePool() {
        tx_byte_pool_delete(&txBytePool);
    }

    TXError releaseMemory(void* memoryPointer) override {
        uint32_t status = tx_byte_release(memoryPointer);
        return static_cast<TXError>(status);
    }

    TXError init() override {
        uint32_t status = tx_byte_pool_create(&txBytePool, name, buffer, SIZE);
        return static_cast<TXError>(status);
    }

    TXError allocateMemory(std::size_t amount, uint32_t waitOption, void** memoryPointer) override {
        uint32_t status = tx_byte_allocate(&txBytePool, memoryPointer, amount, waitOption);
        return static_cast<TXError>(status);
    }

    TXError getAvailableBytes(uint32_t* availableBytes) override {
        uint32_t status =
            tx_byte_pool_info_get(&txBytePool, nullptr, availableBytes, nullptr, nullptr, nullptr, nullptr);
        return static_cast<TXError>(status);
    }

    TXError getFragments(uint32_t* fragments) override {
        uint32_t status = tx_byte_pool_info_get(&txBytePool, nullptr, nullptr, fragments, nullptr, nullptr, nullptr);
        return static_cast<TXError>(status);
    }

    TXError getNameOfFirstSuspendedThread(char** threadName) override {
        TX_THREAD* thread;
        uint32_t status = tx_byte_pool_info_get(&txBytePool, nullptr, nullptr, nullptr, &thread, nullptr, nullptr);
        if (status != SUCCESS)
            return static_cast<TXError>(status);

        status = tx_thread_info_get(thread, threadName, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);

        return static_cast<TXError>(status);
    }

    TXError getNumSuspendedThreads(uint32_t* numSuspendedThreads) override {
        uint32_t status =
            tx_byte_pool_info_get(&txBytePool, nullptr, nullptr, nullptr, nullptr, numSuspendedThreads, nullptr);
        return static_cast<TXError>(status);
    }

    TXError getName(char** name) override {
        *name = this->name;
        return SUCCESS;
    }

private:
    /** A pointer to the name of the Bytepool */
    char* name;

    /** Buffer for the bytepool, SIZE bytes large */
    uint8_t buffer[SIZE];

    /** The struct that the threadx application uses to hold information about the bytepool */
    TX_BYTE_POOL txBytePool;
};

} // namespace core::rtos

#endif //_EVT_RTOS_BYTEPOOLBASE_
