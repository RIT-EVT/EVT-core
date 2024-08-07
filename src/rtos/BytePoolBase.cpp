
#include <EVT/rtos/BytePoolBase.hpp>

namespace core::rtos {

template<size_t SIZE>
BytePoolBase<SIZE>::BytePoolBase(const char* name) : tx_byte_pool_buffer(), name(name) {
    if(tx_byte_pool_create(&tx_app_byte_pool, name, tx_byte_pool_buffer, SIZE) != TX_SUCCESS) {
        //TODO: THROW AN ERROR SOMEHOW
    }
}

template<size_t SIZE>
void* BytePoolBase<SIZE>::AllocateMemory(size_t amount, uint32_t waitOption) {
    void* output;
    tx_byte_allocate(&tx_app_byte_pool, &output, amount, waitOption);
    return output;
}

} //namespace core::rtos
