
#ifndef _EVT_RTOS_Initializeable_
#define _EVT_RTOS_Initializeable_


#include <../../../libs/threadx/common/inc/tx_api.h>
#include <EVT/rtos/BytePoolBase.hpp>
#include <EVT/rtos/Enums.hpp>

namespace core::rtos {

class Initializable {
public:
    /**
     * Method that the threadx init method will call to start this object.
     *
     * @param pool The byte pool that this initializable object will be stored in.
     * @return The first error found by the function (or Success if there was no error).
     */
    virtual TXError init(BytePoolBase &pool) = 0;
};

} //namespace core::rtos

#endif //_EVT_RTOS_Initializeable_
