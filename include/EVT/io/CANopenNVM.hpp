#ifndef _EVT_CANopenNVM_
#define _EVT_CANopenNVM_

#include <Canopen/co_if.h>

namespace EVT::core::IO {

    /**
     * Get an instance of the NVM (non-volotile memory) that can be used
     * with the CANopen stack. This will populate a struct with
     * function points that can handle CAN operations.
     *
     * THIS IS USED FOR TESTING ONLY
     *
     * @param nvmDriver[out] The NVM driver to populate
     */
    void getCANopenNVMDriver(CO_IF_NVM_DRV* nvmDriver);

}  // namespace EVT::core::IO

#endif
