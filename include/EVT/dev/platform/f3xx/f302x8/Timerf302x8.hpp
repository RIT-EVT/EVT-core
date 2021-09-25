#ifndef EVT_TIMER302X8_H
#define EVT_TIMER302X8_H

#include <EVT/dev/platform/Timer.hpp>
#include <cstdint>
#include <HALf3/stm32f3xx_hal_tim.h>

namespace EVT::core::DEV {

/**
 * Implementation of the Timer class for the f302x8 uC
 */
class Timerf302x8: public Timer {
public:

    /**
     * Empty Constructor.  Will startup the Timer
     * @param clockPeriod the clock period in ms.  An interrupt will be triggered at that frequency.
     */
    explicit Timerf302x8(uint32_t clockPeriod);
    // TODO: Set it up so you can configure which timer device is initialized

    void startTimer(void (*irqHandler)(TIM_HandleTypeDef *htim)) override;

    void stopTimer() override;

private:
//    // Instance of the HAL Timer instance
//    TIM_HandleTypeDef halTimer;
};

}  // namespace EVT::core::DEV

#endif //EVT_TIMER302X8_H
