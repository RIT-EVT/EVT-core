
#ifndef _EVT_RTOS_Initializeable_
#define _EVT_RTOS_Initializeable_

namespace core::rtos {

class Initializable {
public:
    /*
     * Method that the threadx init method will call to start this object
     *
     * @return If the initialization succeeded
     */
    bool init();
};

} //namespace core::rtos


#endif //_EVT_RTOS_Initializeable_
