#ifndef _EVT_TIMER3xx_H
#define _EVT_TIMER3xx_H

#include <cstdint>

#include <HALf3/stm32f3xx.h>

#include <core/dev/Timer.hpp>
#include <core/dev/TimerTypes.hpp>

namespace core::dev {

/**
 * Concrete implementation of the Timer class for STM32f3xx MCUs.
 */
class TimerF3xx : public Timer {
public:
    /**
     * The default configuration for an F3 timer.
     */
    static constexpr TimerConfiguration_t defaultConfig = {
        TimerCounterMode::UP,
        TimerClockDivision::DIVISION_1,
        TimerAutoReloadPreload::ENABLE,
        TimerClockSource::INTERNAL,
        TimerMasterModeSelection::RESET,
        TimerMasterSlaveMode::DISABLE
    };
    /**
     * Will initialize the timer device on the STM with the given period and the given IRQ Handler
     * that triggers with the given period.  Starts the timer
     *
     * @param timerPeripheral[in] The timer peripheral to configure. It is up to the user to verify that resource
     * conflicts do not occur. F334 Valid Options: TIM1, TIM2, TIM3, TIM15, TIM16, TIM17. F3O2 Valid Options: TIM1,
     * TIM2, TIM15, TIM16, TIM17.
     * @param[in] timerPeripheral The timer to use
     * @param[in] clockPeriod The clock period in ticks (ms when using AUTO_PRESCALER). An interrupt will be triggered
     * at this frequency.
     * @param[in] configuration The configuration tells the timer how to configure itself. Defaults to @ref
     * TimerF3xx::defaultConfig.
     * @param[in] clockPrescaler The prescaler that the clock will use. If clockPrescaler is set to @ref AUTO_PRESCALER,
     * this function will calculate its own prescaler value.
     */
    explicit TimerF3xx(TIM_TypeDef* timerPeripheral, uint32_t clockPeriod,
                       const TimerConfiguration_t& configuration = defaultConfig,
                       uint32_t clockPrescaler                   = AUTO_PRESCALER);

    /** @inheritDoc */
    void startTimer(void (*irqHandler)(void* context, void* htim), void* context) override;

    /** @inheritDoc */
    void startTimer() override;

    /** @inheritDoc */
    void stopTimer() override;

    /** @inheritDoc */
    void reloadTimer() override;

    /** @inheritDoc */
    void setPeriod(uint32_t clockPeriod, uint32_t clockPrescaler = AUTO_PRESCALER) override;

protected:
    /**
     * Pointer to the halTimer struct stored in the global array in Timerf4xx.cpp
     */
    TIM_HandleTypeDef* halTimer;

    /**
     * Timer clock period
     */
    uint32_t clockPeriod{};

    /**
     * The configuration that this timer will use.
     */
    TimerConfiguration_t configuration;

    /**
     * Handles the initialization of the timer module.  Actually configures the device and enables it.
     *
     * @param timerPeripheral[in] The timer peripheral to configure. It is up to the user to verify that resource
     * conflicts do not occur. F334 Valid Options: TIM1, TIM2, TIM3, TIM15, TIM16, TIM17. F3O2 Valid Options: TIM1,
     * TIM2, TIM15, TIM16, TIM17.
     * @param[in] clockPeriod the clock period in ticks (ms when using AUTO_PRESCALER).  An interrupt will be triggered
     * at that frequency.
     * @param[in] clockPrescaler the prescaler that the clock will use. If clockPrescaler is set to @ref AUTO_PRESCALER,
     * this function will calculate its own prescaler value.
     */
    void initTimer(TIM_TypeDef* timerPeripheral, uint32_t clockPeriod, uint32_t clockPrescaler);
};

} // namespace core::dev

#endif //_EVT_TIMER3xx_H
