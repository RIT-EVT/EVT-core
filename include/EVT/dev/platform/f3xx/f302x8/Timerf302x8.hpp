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
     * Will initialize the timer device on the STM with the given period and the given IRQ Handler
     * that triggers with the given period.  Starts the timer
     * @param clockPeriod the clock period in ms.  An interrupt will be triggered at that frequency.
     * @param irqHandler Function pointer to the IRQ Handler desired for the given timer.
     */
    explicit Timerf302x8(uint32_t clockPeriod, void (*irqHandler)(TIM_HandleTypeDef *htim));
    // TODO: Set it up so you can configure which timer device is initialized

    void startTimer(void (*irqHandler)(TIM_HandleTypeDef *htim)) override;

    void startTimer() override;

    void stopTimer() override;

private:
//    // Instance of the HAL Timer instance
//    TIM_HandleTypeDef halTimer;

    // Timer clock period
    uint32_t clockPeriod;

    /**
     * Handles the initialization of the timer module.  Actually configures the device and enables it.
     * @param clockPeriod the clock period in ms.  An interrupt will be triggered at that frequency.
     */
    void initTimer(uint32_t clockPeriod);
};

}  // namespace EVT::core::DEV

#endif //EVT_TIMER302X8_H
