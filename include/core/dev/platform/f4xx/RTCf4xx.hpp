#ifndef _EVT_RTCf4xx_
#define _EVT_RTCf4xx_

#include <HALf4/stm32f4xx.h>

#include <core/dev/RTC.hpp>

namespace core::dev {

/**
 * RTC implementation that makes use of the on-board real time clock
 * on STM32f3xx MCUs.
 *
 * The onboard RTC can be kept up to date with a coin cell battery.
 */
class RTCf4xx : public RTC {
public:
    /**
     * Empty constructor. Will startup the RTC.
     */
    RTCf4xx();

    void getTime(core::time::TimeStamp& time) override;

    uint32_t getTime() override;

    void setTime(core::time::TimeStamp& time) override;

private:
    /** Instance of the HAL RTC interface */
    RTC_HandleTypeDef halRTC;
};

} // namespace core::dev

#endif
