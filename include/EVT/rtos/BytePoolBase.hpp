
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
     * Constructs a BytePool, including creating a buffer to hold the
     * information for the BytePool and the buffer for the pool itself.
     */
    BytePoolBase();


    void* AllocateMemory(std::size_t amount, bool wait) override;

private:
    UCHAR tx_byte_pool_buffer[SIZE];
    TX_BYTE_POOL tx_app_byte_pool;
};

} //namespace core::rtos


#endif //_EVT_RTOS_BYTEPOOLBASE_
