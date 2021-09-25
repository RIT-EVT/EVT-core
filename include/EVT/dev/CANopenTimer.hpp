#ifndef _EVT_CANopenTIMER_
#define _EVT_CANopenTIMER_

#include <EVT/dev/RTC.hpp>

#include <Canopen/co_if.h>

namespace EVT::core::DEV {

    /**
     * Get an instance of the timer driver that can be used with the CANopen
     * stack. This will populate a struct with function pointers that
     * can handle timer operations.
     *
     * @param rtc[in] Real time clock that will be used for emulating
     *  timer applications
     * @param timerDriver[out] The timer driver to populate
     */
    void getCANopenTimerDriver(RTC& rtc, CO_IF_TIMER_DRV* timerDriver);

}  // namespace EVT::core::IO

#endif
