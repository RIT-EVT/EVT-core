#ifndef _EVT_RTCf302x8_
#define _EVT_RTCf302x8_

#include <HALf3/stm32f3xx.h>

#include <EVT/dev/RTC.hpp>

namespace EVT::core::DEV {

/**
 * RTC implementation that makes use of the on-board real time clock
 * on the STM32f302x8.
 *
 * The onboard RTC can be kept up to date with a coin cell battery.
 */
class RTCf302x8 : public RTC {
public:
    /**
     * Empty constructor. Will startup the RTC.
     */
    RTCf302x8();

    /**
     * Get the current time as determined by the real time clock.
     *
     * @param[out] time The time struct to populate
     */
    void getTime(EVT::core::time::TimeStamp& time);

    /**
     * Get the timestamp since epoch in seconds
     *
     * @return The time since epoch as determined by the RTC
     */
    uint32_t getTime();

    /**
     * Set the time of the real time clock.
     *
     * @param[in] time The time to set the RTC to use.
     */
    void setTime(EVT::core::time::TimeStamp& time);

private:
    /** Instance of the HAL RTC interface */
    RTC_HandleTypeDef halRTC;
};

}// namespace EVT::core::DEV

#endif
