#ifndef _EVT_PWMf3xx_
#define _EVT_PWMf3xx_

#include <stdint.h>

#include <HALf3/stm32f3xx.h>
#include <core/dev/platform/f3xx/Timerf3xx.hpp>
#include <core/io/PWM.hpp>

namespace core::io {

class PWMf3xx : public PWM, public dev::TimerF3xx {
public:
    /**
     * Setup the given pin for PWM usage.
     *
     * @param pin[in] The pin to setup for PWM
     */
    PWMf3xx(Pin pin, TIM_TypeDef* timerPeripheral, uint32_t clockPeriod, dev::TimerConfiguration configuration, uint32_t clockPrescaler = AUTO_PRESCALER);

    void setDutyCycle(uint32_t dutyCycle);

    void setPeriod(uint32_t period);

    uint32_t getDutyCycle();

    uint32_t getPeriod();
private:
    /// Channel identification
    uint32_t halTIMChannelID {};
};

} // namespace core::io

#endif
