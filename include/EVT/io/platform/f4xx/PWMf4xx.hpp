#ifndef _EVT_PWMf4xx_
#define _EVT_PWMf4xx_

#include <stdint.h>

#include <EVT/io/PWM.hpp>
#include <EVT/io/pin.hpp>
#include <HALf4/stm32f4xx.h>

namespace EVT::core::IO {

class PWMf4xx : public PWM {
public:
    /**
     * Setup the given pin for PWM usage.
     *
     * @param pin[in] The pin to setup for PWM
     */
    PWMf4xx(Pin pin);

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

}// namespace EVT::core::IO

#endif
