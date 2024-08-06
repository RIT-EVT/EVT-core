/**
 * A pure virtual class that allows us to pass the BytePoolBase template class into external methods
 * without the compiler complaining. After initialization, the BytePoolBase class should always be referenced
 * as a BytePool, and never as a BytePoolBase. There should be no methods that accept a BytePoolBase as an argument.
 */

#ifndef _EVT_RTOS_BYTEPOOL_
#define _EVT_RTOS_BYTEPOOL_

#include <EVT/rtos/BytePoolBase.hpp>
#include <cstdint>

namespace core::rtos {

class BytePool {
public:

    /**
     * Allocates memory from the BytePool and returns a pointer to the start of it.
     *
     * @param amount how much memory (in bytes) that is requested.
     * @param wait If the calling thread should wait for the memory to be available.
     * @return a pointer to the allocated bytepool memory.
     */
    virtual void* AllocateMemory(std::size_t amount, bool wait) = 0;
};

} //namespace core::rtos

#endif _EVT_RTOS_BYTEPOOL_