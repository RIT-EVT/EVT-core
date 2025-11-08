#ifndef _EVT_TIMER4xx_H
#define _EVT_TIMER4xx_H

#include <cstdint>

#include <HALf4/stm32f4xx.h>

#include <core/dev/Timer.hpp>

namespace core::dev {

/**
 * Concrete implementation of the Timer class for STM32f4xx MCUs.
 */
class TimerF4xx final : public Timer {
public:
    /**
     * A default configuration for an F4 timer.
     */
    static constexpr TimerConfiguration_t defaultConfig = {TIM_COUNTERMODE_UP,
                                                           TIM_CLOCKDIVISION_DIV1,
                                                           TIM_AUTORELOAD_PRELOAD_ENABLE,
                                                           TIM_CLOCKSOURCE_INTERNAL,
                                                           TIM_TRGO_RESET,
                                                           TIM_MASTERSLAVEMODE_DISABLE};

    ~TimerF4xx() override;

    /**
     * Will initialize the timer device on the STM with the given period and the given IRQ Handler
     * that triggers with the given period.  Starts the timer
     * @param timerPeripheral[in] The timer peripheral to configure. Possible options for this board are
     * TIM1, TIM2, TIM3, TIM4, TIM5, TIM8, TIM9, TIM10, TIM11, TIM11, TIM13, TIM14.
     * It is up to the user to verify that resource conflicts do not occur.
     *
     * @param[in] timerPeripheral the timer peripheral to use.
     * @param[in] clockPeriod the clock period in ticks (ms when using AUTO_PRESCALER). An interrupt will be triggered
     * at this frequency.
     * @param[in] configuration the configuration tells the timer how to configure itself. Defaults to @ref
     * TimerF4xx::defaultConfig.
     * @param[in] clockPrescaler the prescaler that the clock will use. If clockPrescaler is set to @ref AUTO_PRESCALER,
     * this function will calculate its own prescaler value.
     */
    explicit TimerF4xx(TIM_TypeDef* timerPeripheral, uint32_t clockPeriod,
                       const TimerConfiguration_t& configuration = defaultConfig,
                       uint32_t clockPrescaler                   = AUTO_PRESCALER);

    /**
     * Starts the given timer and registers the given interrupt pointer to trigger when the timer overflows
     * @param[in] irqHandler The IRQ Handler function pointer.  Sets a new interrupt handler function
     */
    void startTimer(void (*irqHandler)(void* context, void* htim), void* context) override;

    /**
     * Starts the given timer using the IRQ Handler already assigned to that timer.
     */
    void startTimer() override;

    /**
     * Stops the current timer from running.  Does not complete its current counting sequence.
     */
    void stopTimer() override;

    /**
     * Resets the timer counter.
     */
    void reloadTimer() override;

    /**
     * Set the clock period for the timer.  Will stop the timer, re-initialize the device with the updated period.
     * You must call startTimer again to continue timer operation.
     *
     * @param[in] clockPeriod the clock period in ticks (ms when using AUTO_PRESCALER).  An interrupt will be triggered
     * at that frequency.
     * @param [in] clockPrescaler the prescaler used by the clock. Divides the system clock frequency to get it within
     * an acceptable range for clocking. If set to @ref AUTO_PRESCALER, the function implementation should calculate its
     * own prescaler.
     */
    void setPeriod(uint32_t clockPeriod, uint32_t clockPrescaler = AUTO_PRESCALER) override;

protected:
    /**
     * Pointer to the halTimer struct stored in the global array in Timerf4xx.cpp
     */
    TIM_HandleTypeDef* halTimer = {};

    /**
     * Timer clock period
     */
    uint32_t clockPeriod;

    /**
     * The configuration that this timer will use.
     */
    TimerConfiguration_t configuration;

    /**
     * Handles the initialization of the timer module.  Actually configures the device and enables it.
     * @param[in] timerPeripheral  The timer peripheral to configure.  Possible options for this board are
     * TIM2, TIM15, TIM16, TIM17.  It is up to the user to verify that resource conflicts do not occur.
     * @param[in] clockPeriod the clock period in ticks (ms when using AUTO_PRESCALER).  An interrupt will be triggered
     * at that frequency.
     * @param[in] clockPrescaler the prescaler that the clock will use. If clockPrescaler is set to @ref AUTO_PRESCALER,
     * this function will calculate its own prescaler value.
     */
    void initTimer(TIM_TypeDef* timerPeripheral, uint32_t clockPeriod, uint32_t clockPrescaler);
};
} // namespace core::dev

#endif //_EVT_TIMER4xx_H