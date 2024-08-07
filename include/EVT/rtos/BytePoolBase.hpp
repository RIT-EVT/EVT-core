/**
 * A template class that holds the actual information for managing a threadx bytepool.
 */

#ifndef _EVT_RTOS_BYTEPOOLBASE_
#define _EVT_RTOS_BYTEPOOLBASE_

#include <EVT/rtos/BytePool.hpp>
#include <../../../libs/threadx/common/inc/tx_api.h>
#include <../../../libs/threadx/ports/cortex_m4/gnu/inc/tx_port.h>
#include <cstdint>

namespace core::rtos {

template <std::size_t SIZE>
class BytePoolBase : BytePool {
public:
    /**
     * Constructs a BytePoolBase, including creating a buffer to hold the
     * information for the pool and the buffer for the pool itself.
     */
    BytePoolBase(const char* name);

    void* AllocateMemory(std::size_t amount, uint32_t waitOption) override;

private:
    /**
     * Buffer for the bytepool.
     */
    UCHAR tx_byte_pool_buffer[SIZE];
    /**
     * The struct that the threadx application uses to hold information about the bytepool.
     */
    TX_BYTE_POOL tx_app_byte_pool;
    const char* name;
};

} //namespace core::rtos


#endif //_EVT_RTOS_BYTEPOOLBASE_
