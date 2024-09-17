#ifndef _EVT_TIMER4xx_H
#define _EVT_TIMER4xx_H

#include <cstdint>

#include <HALf4/stm32f4xx.h>

#include <core/dev/Timer.hpp>

namespace core::dev {

/**
 * Implementation of the Timer class for STM32f4xx MCUs
 */
class Timerf4xx : public Timer {
public:
    /**
     * Will initialize the timer device on the STM with the given period and the given IRQ Handler
     * that triggers with the given period.
     * @param timerPeripheral[in] The timer peripheral to configure.  It is up to the user to verify
     * that resource conflicts do not occur.
     *
     * @param[in] timerPeripheral The timer to use
     * @param[in] clockPeriod the clock period in ms.  An interrupt will be triggered at that frequency.
     */
    explicit Timerf4xx(TIM_TypeDef* timerPeripheral, uint32_t clockPeriod);

    void startTimer(void (*irqHandler)(void* htim)) override;

    void startTimer() override;

    void stopTimer() override;

    void reloadTimer() override;

    void setPeriod(uint32_t clockPeriod) override;

private:
    // Pointer to the halTimer struct stored in the global array in Timerf4xx.cpp
    TIM_HandleTypeDef* halTimer;

    // Timer clock period
    uint32_t clockPeriod;

    /**
     * Handles the initialization of the timer module.  Actually configures the device and enables it.
     * @param[in] timerPeripheral  The timer peripheral to configure.
     * @param[in] clockPeriod the clock period in ms.  An interrupt will be triggered at that frequency.
     */
    void initTimer(TIM_TypeDef* timerPeripheral, uint32_t clockPeriod);
};

} // namespace core::dev

#endif //_EVT_TIMER4xx_H
