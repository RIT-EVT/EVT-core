#ifndef _EVT_TIMER4xx_H
#define _EVT_TIMER4xx_H

#include <cstdint>

#include <HALf4/stm32f4xx.h>

#include <core/dev/Timer.hpp>

namespace core::dev {

class TimerF4xx final : public Timer {
public:
    ~TimerF4xx() override;

    explicit TimerF4xx(TIM_TypeDef* timerPeripheral, uint32_t clockPeriod, TimerConfiguration configuration);

    void startTimer(void (*irqHandler)(void* htim)) override;

    void startTimer() override;

    void stopTimer() override;

    void reloadTimer() override;

    void setPeriod(uint32_t clockPeriod) override;
private:
    // Pointer to the halTimer struct stored in the global array in Timerf4xx.cpp
    TIM_HandleTypeDef* halTimer = { };

    // Timer clock period
    uint32_t clockPeriod;

    TimerConfiguration configuration;

    /**
     * Handles the initialization of the timer module.  Actually configures the device and enables it.
     * @param[in] timerPeripheral  The timer peripheral to configure.
     * @param[in] clockPeriod the clock period in ms.  An interrupt will be triggered at that frequency.
     * @param configuration
     */
    void initializeTimer(TIM_TypeDef *timerPeripheral, uint32_t clockPeriod);
};
} // namespace core::dev

#endif //_EVT_TIMER4xx_H