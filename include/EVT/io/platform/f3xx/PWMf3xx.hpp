#ifndef _EVT_PWMf3xx_
#define _EVT_PWMf3xx_

#include <stdint.h>

#include <EVT/io/PWM.hpp>
#include <EVT/io/pin.hpp>
#include <HALf3/stm32f3xx.h>

namespace EVT::core::IO {

class PWMf3xx : public PWM {
public:
    /**
     * Setup the given pin for PWM usage.
     *
     * @param pin[in] The pin to setup for PWM
     */
    PWMf3xx(Pin pin);

    void setDutyCycle(float dutyCycle);

    void setPeriod(float period);

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
