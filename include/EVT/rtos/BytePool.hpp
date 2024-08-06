
#ifndef _EVT_RTOS_BYTEPOOL_
#define _EVT_RTOS_BYTEPOOL_

#include <EVT/rtos/BytePoolBase.hpp>
#include <cstdint>

namespace core::rtos {

class BytePool {
public:
    virtual void* AllocateMemory(std::size_t amount, bool wait) = 0;
};

} //namespace core::rtos

#endif _EVT_RTOS_BYTEPOOL_