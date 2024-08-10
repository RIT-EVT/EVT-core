#ifndef _EVT_RTCf3xx_
#define _EVT_RTCf3xx_

#include <HALf3/stm32f3xx.h>

#include <core/dev/RTC.hpp>

namespace core::DEV {

/**
 * RTC implementation that makes use of the on-board real time clock
 * on STM32f3xx MCUs.
 *
 * The onboard RTC can be kept up to date with a coin cell battery.
 */
class RTCf3xx : public RTC {
public:
    /**
     * Empty constructor. Will startup the RTC.
     */
    RTCf3xx();

    void getTime(core::time::TimeStamp& time) override;

    uint32_t getTime() override;

    void setTime(core::time::TimeStamp& time) override;

private:
    /** Instance of the HAL RTC interface */
    RTC_HandleTypeDef halRTC;
};

}// namespace core::DEV

#endif
