
#include <EVT/rtos/BytePool.hpp>

namespace core::rtos {

template<size_t SIZE>
BytePool<SIZE>::BytePool(const char* name)
    : tx_byte_pool_buffer(), name(name) {}

template<size_t SIZE>
TXError BytePool<SIZE>::init() {
    return tx_byte_pool_create(&tx_app_byte_pool, name, tx_byte_pool_buffer, SIZE);
}

template<size_t SIZE>
void* BytePool<SIZE>::AllocateMemory(size_t amount, uint32_t waitOption) {
    void* output;
    tx_byte_allocate(&tx_app_byte_pool, &output, amount, waitOption);
    //TODO: this implementation swallows the error code that tx_byte_allocate can return
    return output;
}

} //namespace core::rtos
