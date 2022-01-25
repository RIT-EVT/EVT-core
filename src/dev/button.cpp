#include <EVT/dev/button.hpp>
#include <HALf3/stm32f3xx_hal.h>

namespace EVT::core::DEV {
    Button::Button(EVT::core::IO::GPIO &gpio, Button::LogicLevelOnPress logicLevelOnPress) : gpio(gpio),
                                                                                             logicLevelOnPress(
                                                                                                     logicLevelOnPress) {}

    /**
     * Reads the button state and returns it
     *
     * @return Button State
     */
    IO::GPIO::State Button::readButton() {

        if (this->logicLevelOnPress == Button::LogicLevelOnPress::HIGH) {
            return this->gpio.readPin();
        } else {
            auto state = gpio.readPin();
            if (state == EVT::core::IO::GPIO::State::HIGH) {
                return EVT::core::IO::GPIO::State::LOW;
            } else {
                return EVT::core::IO::GPIO::State::HIGH;
            }
        }
    }


    /**
     * Confirms if there was a press based on how long a signal is
     * @param logicLevelOnPress
     * @return bool representing a confirmed press
     */

    IO::GPIO::State Button::debounce(uint32_t debounceStart, uint32_t debounceTimeReq) {
        if (HAL_GetTick() - debounceStart > debounceTimeReq) {
            return this->readButton();
        } else {
            if (this->readButton() == EVT::core::IO::GPIO::State::HIGH) {
                return EVT::core::IO::GPIO::State::LOW;

            } else {
                return EVT::core::IO::GPIO::State::HIGH;
            }
        }
    }


}// namespace EVT::core::DEV
