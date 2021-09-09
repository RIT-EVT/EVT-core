#ifndef _EVT_RTCf302x8_
#define _EVT_RTCf302x8_

#include <EVT/dev/RTC.hpp>

namespace EVT::core::DEV {


/**
 * RTC implementation that makes use of the on-board real time clock
 * on the STM32f302x8.
 *
 * The onboard RTC can be kept up to date with a coin cell battery.
 */
class RTCf302x8: public RTC {
public:
    /**
     * Get the current time as determined by the real time clock.
     *
     * @param[out] time The time struct to populate
     */
    void getTime(EVT::core::time::TimeStamp time);

    /**
     * Get the timestamp since epoch in seconds
     *
     * @return The time since epoch as determined by the RTC
     */
    uint64_t getTime();

    /**
     * Set the time of the real time clock.
     *
     * @param[in] time The time to set the RTC to use.
     */
    void setTime(EVT::core::time::TimeStamp time);


};


}  // namespace EVT::core::DEV

#endif
