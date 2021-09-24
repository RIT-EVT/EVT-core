#ifndef EVT_TIMER302X8_H
#define EVT_TIMER302X8_H

#include <EVT/dev/platform/Timer.hpp>

namespace EVT::core::DEV {

/**
 * Implementation of the Timer class for the f302x8 uC
 */
class Timerf302x8: public Timer {
public:

    /**
     * Empty Constructor.  Will startup the Timer
     */
    Timerf302x8();
    // TODO: Set it up so you can configure which timer device is initialized

    void startTimer(uint32_t timerPeriod, void (*irqHandler)()) override;

    void stopTimer() override;

private:
//    // Instance of the HAL Timer instance
//    TIM_HandleTypeDef halTimer;
};

}  // namespace EVT::core::DEV

#endif //EVT_TIMER302X8_H
