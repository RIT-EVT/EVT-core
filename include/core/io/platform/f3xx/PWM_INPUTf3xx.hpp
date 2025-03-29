
#ifndef EVT_PWM_INPUTF3XX_HPP
#define EVT_PWM_INPUTF3XX_HPP

#include <stdint.h>

#include <HALf3/stm32f3xx.h>
#include <core/io/PWM_INPUT.hpp>
#include <core/io/pin.hpp>

namespace core::io {

class PWM_INPUTf3xx : public PWM_INPUT {
public:
    /**
     * Setup the given pin for PWM usage.
     *
     * @param pin[in] The pin to setup for PWM
     */
    PWM_INPUTf3xx(Pin pin);

    uint32_t getDutyCycle();

    uint32_t getPeriod();

    uint32_t getFrequency();

private:
    /// HAL timer representation
    TIM_HandleTypeDef halTIM;
    /// Channel identification
    uint32_t halTIMChannelID;
    /// HAL channel representation
    TIM_OC_InitTypeDef halChannel;
};

} // namespace core::io

#endif // EVT_PWM_INPUTF3XX_HPP
