#ifndef _EVT_RTOS_BYTEPOOLBASE_
#define _EVT_RTOS_BYTEPOOLBASE_

#include <core/rtos/BytePoolBase.hpp>
#include <cstdint>
#include <cstring>

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
     * @param[in] name String name of the BytePool, should be no longer than BYTEPOOL_NAME_MAX_LENGTH bytes.
     * The name is copied into this object
     */
    explicit BytePool(char* name) : txBytePool(), buffer() {
        strncpy(this->name, name, BYTEPOOL_NAME_MAX_LENGTH);
        this->name[BYTEPOOL_NAME_MAX_LENGTH] = '\0';
    }

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
        if (status != TXE_SUCCESS) {
            return static_cast<TXError>(status);
        }

        status = tx_thread_info_get(thread, threadName, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);

        return static_cast<TXError>(status);
    }

    TXError getNumSuspendedThreads(uint32_t* numSuspendedThreads) override {
        uint32_t status =
            tx_byte_pool_info_get(&txBytePool, nullptr, nullptr, nullptr, nullptr, numSuspendedThreads, nullptr);
        return static_cast<TXError>(status);
    }

    void getName(char* destination, size_t size) override {
        if (size > BYTEPOOL_NAME_MAX_LENGTH + 1) {
            size = BYTEPOOL_NAME_MAX_LENGTH + 1;
        }
        strncpy(destination, this->name, size);
        destination[size - 1] = 0;
    }

private:
    /** The name of the Bytepool */
    char name[BYTEPOOL_NAME_MAX_LENGTH] = {};

    /** The struct that the threadx application uses to hold information about the bytepool */
    TX_BYTE_POOL txBytePool;

    /** Buffer for the bytepool, SIZE bytes large */
    uint8_t buffer[SIZE];
};

} // namespace core::rtos

#endif //_EVT_RTOS_BYTEPOOLBASE_
