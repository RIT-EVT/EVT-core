#ifndef _EVT_CANopen_
#define _EVT_CANopen_

#include <Canopen/co_if.h>

#include <EVT/io/CAN.hpp>

namespace EVT::core::IO {

   /**
    * Get an instance of the CAN driver that can be used with the CANopen
    * stack. This will populate a struct with function pointers that can
    * handle CAN operations.
    *
    * @param can[in] The CAN interface that will be used for the stack driver
    * @param canDriver[out] The CANopen stack driver to populate
    */
    void getCANopenDriver(CAN& can, CO_IF_CAN_DRV* canDriver);



}  // namespace EVT::core::IO


#endif
