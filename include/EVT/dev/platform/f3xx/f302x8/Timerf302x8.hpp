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
     * @param timerPeripheral The timer peripheral to configure.  Possible options for this board are
     * TIM2, TIM15, TIM16, TIM17.  It is up to the user to verify that resource conflicts
     * do not occur.
     * @param clockPeriod the clock period in ms.  An interrupt will be triggered at that frequency.
     * @param irqHandler Function pointer to the IRQ Handler desired for the given timer.
     */
    explicit Timerf302x8(TIM_TypeDef *timerPeripheral, uint32_t clockPeriod, void (*irqHandler)(TIM_HandleTypeDef *htim));

    void startTimer(void (*irqHandler)(TIM_HandleTypeDef *htim)) override;

    void startTimer() override;

    void stopTimer() override;

private:
    // Pointer to the halTimer struct stored in the global of Timerf302x8.cpp
    TIM_HandleTypeDef *halTimer;

    // Timer clock period
    uint32_t clockPeriod;

    /**
     * Handles the initialization of the timer module.  Actually configures the device and enables it.
     * @param clockPeriod the clock period in ms.  An interrupt will be triggered at that frequency.
     */
    void initTimer(TIM_TypeDef *timerPeripheral, uint32_t clockPeriod);
};

}  // namespace EVT::core::DEV

#endif //EVT_TIMER302X8_H
