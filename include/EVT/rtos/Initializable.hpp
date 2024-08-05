
#ifndef _EVT_RTOS_Initializeable_
#define _EVT_RTOS_Initializeable_

#include <EVT/rtos/BytePool.hpp>

namespace core::rtos {

class Initializable {
public:
    /**
     * Method that the threadx init method will call to start this object.
     *
     * @param pool The byte pool that this initializable object will be stored in.
     * @return If the initialization succeeded.
     */
    bool init(BytePool &pool);
};

} //namespace core::rtos


#endif //_EVT_RTOS_Initializeable_
