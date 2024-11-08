#ifndef _EVT_RTOS_INITIALIZABLE_
#define _EVT_RTOS_INITIALIZABLE_

#include <core/rtos/BytePoolBase.hpp>
#include <core/rtos/Enums.hpp>
#include <tx_api.h>

namespace core::rtos {

/**
 * Abstract class that represents a ThreadX wrapper object that must be passed into the
 * Threadx::startKernel() method as part of the initList parameter.
 */
class Initializable {
public:
    /**
     * Initialize this object with the kernel
     *
     * @param pool The byte pool that this initializable object will be stored in.
     * @return The first error found by the function (or Success if there was no error).
     */
    virtual TXError init(BytePoolBase& pool) = 0;
};

} // namespace core::rtos

#endif //_EVT_RTOS_INITIALIZABLE_
