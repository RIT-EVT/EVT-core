/**
 * Contains functions to make CANopen stack versions of drivers. The
 * functions will take in an EVT-core driver which will be used for
 * making the drivers.
 */
#ifndef _EVT_CANopen_
#define _EVT_CANopen_

#include <Canopen/co_if.h>

#include <EVT/io/CAN.hpp>
#include <EVT/dev/Timer.hpp>

namespace EVT::core::IO {

   /**
    * Get an instance of the CAN driver that can be used with the CANopen
    * stack. This will populate a struct with function pointers that can
    * handle CAN operations.
    *
    * @param can[in] The CAN interface that will be used for the stack driver
    * @param canDriver[out] The CANopen stack driver to populate
    */
    void getCANopenCANDriver(CAN& can, CO_IF_CAN_DRV* canDriver);

    /**
     * Get an instance of the timer driver that can be used with the
     * CANopen stack. This will populate a struct with function pointers
     * that will handler timer operations.
     *
     * @param timer[in] The timer interface to use
     * @param timerDriver[ou]t The timer driver to populate
     */
    void getCANopenTimerDriver(DEV::Timer& timer, CO_IF_TIMER_DRV* timerDriver);

    /**
     * Get an instance of the NVM (non-volitile memory) that can be used
     * with the CANopen stack. This will populate a struct with
     * function pointers that can handle CAN operations.
     *
     * CURRENTLY ONLY FOR TESTING USE ONLY, NOT ACTUALLY NVM.
     *
     * @param nvmDriver[out] The NVM driver to populate
     */
    void getCANopenNVMDriver(CO_IF_NVM_DRV* nvmDriver);

}  // namespace EVT::core::IO


#endif
