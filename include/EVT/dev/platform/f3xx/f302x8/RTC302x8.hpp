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

    void getTime(EVT::core::time::TimeStamp& time);

    uint32_t getTime();

    void setTime(EVT::core::time::TimeStamp& time);

private:
    /** Instance of the HAL RTC interface */
    RTC_HandleTypeDef halRTC;
};

}// namespace EVT::core::DEV

#endif
