#ifndef _EVT_TIMER3xx_H
#define _EVT_TIMER3xx_H

#include <cstdint>

#include <HALf3/stm32f3xx.h>

#include <core/dev/Timer.hpp>

namespace core::dev {

/**
 * Implementation of the Timer class for STM32f3xx MCUs
 */
class Timerf3xx : public Timer {
public:
    /**
     * Will initialize the timer device on the STM with the given period and the given IRQ Handler
     * that triggers with the given period.  Starts the timer
     * @param timerPeripheral[in] The timer peripheral to configure.  Possible options for this board are
     * TIM2, TIM15, TIM16, TIM17.  It is up to the user to verify that resource conflicts
     * do not occur.
     *
     * @param[in] timerPeripheral The timer to use
     * @param[in] clockPeriod the clock period in ms.  An interrupt will be triggered at that frequency.
     */
    explicit Timerf3xx(TIM_TypeDef* timerPeripheral, uint32_t clockPeriod);

    void startTimer(void (*irqHandler)(void* htim)) override;

    void startTimer() override;

    void stopTimer() override;

    void reloadTimer() override;

    void setPeriod(uint32_t clockPeriod) override;

private:
    // Pointer to the halTimer struct stored in the global array in Timerf3xx.cpp
    TIM_HandleTypeDef* halTimer;

    // Timer clock period
    uint32_t clockPeriod;

    /**
     * Handles the initialization of the timer module.  Actually configures the device and enables it.
     * @param[in] timerPeripheral  The timer peripheral to configure.  Possible options for this board are
     * TIM2, TIM15, TIM16, TIM17.  It is up to the user to verify that resource conflicts do not occur.
     * @param[in] clockPeriod the clock period in ms.  An interrupt will be triggered at that frequency.
     */
    void initTimer(TIM_TypeDef* timerPeripheral, uint32_t clockPeriod);
};

} // namespace core::dev

#endif //_EVT_TIMER3xx_H
