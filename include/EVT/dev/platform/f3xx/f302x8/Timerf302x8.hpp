#ifndef EVT_TIMER302X8_H
#define EVT_TIMER302X8_H

#include <EVT/dev/Timer.hpp>
#include <HALf3/stm32f3xx_hal_tim.h>
#include <cstdint>

namespace EVT::core::DEV {

/**
 * Implementation of the Timer class for the f302x8 uC
 */
class Timerf302x8 : public Timer {
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
    explicit Timerf302x8(TIM_TypeDef* timerPeripheral, uint32_t clockPeriod);

    void startTimer(void (*irqHandler)(void* htim)) override;

    void startTimer() override;

    void stopTimer() override;

    void reloadTimer() override;

    virtual void setPeriod(uint32_t clockPeriod);

private:
    // Pointer to the halTimer struct stored in the global of Timerf302x8.cpp
    TIM_HandleTypeDef* halTimer;

    // Timer clock period
    uint32_t clockPeriod;

    /**
     * Handles the initialization of the timer module.  Actually configures the device and enables it.
     * @param timerPeripheral[in]  The timer peripheral to configure.  Possible options for this board are
     * TIM2, TIM15, TIM16, TIM17.  It is up to the user to verify that resource conflicts
     * @param[in] clockPeriod the clock period in ms.  An interrupt will be triggered at that frequency.
     */
    void initTimer(TIM_TypeDef* timerPeripheral, uint32_t clockPeriod);
};

}// namespace EVT::core::DEV

#endif//EVT_TIMER302X8_H
