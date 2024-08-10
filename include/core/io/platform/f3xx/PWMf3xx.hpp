#ifndef _EVT_PWMf3xx_
#define _EVT_PWMf3xx_

#include <stdint.h>

#include <HALf3/stm32f3xx.h>
#include <core/io/PWM.hpp>
#include <core/io/pin.hpp>

namespace core::IO {

class PWMf3xx : public PWM {
public:
    /**
     * Setup the given pin for PWM usage.
     *
     * @param pin[in] The pin to setup for PWM
     */
    PWMf3xx(Pin pin);

    void setDutyCycle(uint32_t dutyCycle);

    void setPeriod(uint32_t period);

    uint32_t getDutyCycle();

    uint32_t getPeriod();

private:
    /// HAL timer representation
    TIM_HandleTypeDef halTIM;
    /// Channel identification
    uint32_t halTIMChannelID;
    /// HAL channel representation
    TIM_OC_InitTypeDef halChannel;
};

}// namespace core::IO

#endif
