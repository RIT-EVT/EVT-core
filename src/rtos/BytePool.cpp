
#include <EVT/rtos/BytePool.hpp>
#include <EVT/rtos/BytePoolBase.hpp>

namespace core::rtos {

template<std::size_t SIZE>
BytePool<SIZE>::BytePool(char* name)
    : txBytePool(), name(name), buffer() {}

template<std::size_t SIZE>
BytePool<SIZE>::~BytePool() {
    tx_byte_pool_delete(&txBytePool);
}

template<std::size_t SIZE>
TXError BytePool<SIZE>::releaseMemory(void * memoryPointer) {
    return static_cast<TXError>(tx_byte_release(memoryPointer));
}

template<std::size_t SIZE>
TXError BytePool<SIZE>::init() {
    return static_cast<TXError>(tx_byte_pool_create(&txBytePool, name, buffer, SIZE));
}

template<std::size_t SIZE>
TXError BytePool<SIZE>::allocateMemory(std::size_t amount, void** memoryPointer, uint32_t waitOption) {
    uint32_t errorCode = tx_byte_allocate(&txBytePool, memoryPointer, amount, waitOption);
    return static_cast<TXError>(errorCode);
}

template<std::size_t SIZE>
TXError BytePool<SIZE>::getAvailableBytes(uint32_t* availableBytes) {
    uint32_t status = tx_byte_pool_info_get(&txBytePool, nullptr, availableBytes,
                          nullptr, nullptr, nullptr, nullptr);
    return static_cast<TXError>(status);
}

template<std::size_t SIZE>
TXError BytePool<SIZE>::getFragments(uint32_t* fragments) {
    uint32_t status = tx_byte_pool_info_get(&txBytePool, nullptr, nullptr,
                                            fragments, nullptr, nullptr, nullptr);
    return static_cast<TXError>(status);
}

template<std::size_t SIZE>
TXError BytePool<SIZE>::getNameOfFirstSuspendedThread(char** name) {
    TX_THREAD* thread;
    uint32_t status = tx_byte_pool_info_get(&txBytePool, nullptr, nullptr,
                                            nullptr, &thread, nullptr, nullptr);
    if (status != Success)
        return static_cast<TXError>(status);

    status = tx_thread_info_get(thread, name, nullptr, nullptr, nullptr,
                                nullptr, nullptr, nullptr, nullptr);

    return static_cast<TXError>(status);
}

template<std::size_t SIZE>
TXError BytePool<SIZE>::getNumSuspendedThreads(uint32_t* numSuspendedThreads) {
    uint32_t status = tx_byte_pool_info_get(&txBytePool, nullptr, nullptr,
                                            nullptr, nullptr, numSuspendedThreads, nullptr);
    return static_cast<TXError>(status);
}

template<std::size_t SIZE>
TXError BytePool<SIZE>::getName(char** name) {
    *name = this->name;
    return Success;
}

} //namespace core::rtos
