#ifndef _EVT_PWMf4xx_
#define _EVT_PWMf4xx_

#include <stdint.h>

#include <HALf4/stm32f4xx.h>
#include <core/io/PWM.hpp>
#include <core/io/pin.hpp>

namespace core::io {

class PWMf4xx : public PWM {
public:
    /**
     * Setup the given pin for PWM usage.
     *
     * @param pin[in] The pin to setup for PWM
     */
    PWMf4xx(Pin pin);

    void setDutyCycle(uint32_t dutyCycle) override;

    void setPeriod(uint32_t period) override;

    uint32_t getDutyCycle() override;

    uint32_t getPeriod() override;

private:
    /// HAL timer representation
    TIM_HandleTypeDef halTIM;
    /// Channel identification
    uint32_t halTIMChannelID;
    /// HAL channel representation
    TIM_OC_InitTypeDef halChannel;
};

} // namespace core::io

#endif
