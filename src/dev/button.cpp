#include <EVT/dev/button.hpp>

namespace EVT::core::DEV {
Button::Button(EVT::core::IO::GPIO& gpio, Button::LogicLevelOnPress logicLevelOnPress) : gpio(gpio), logicLevelOnPress(logicLevelOnPress) {}

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
    Button::LogicLevelOnPress Button::debounce(LogicLevelOnPress logicLevelOnPress, uint32_t debounceTimeReq) {
        uint32_t debounceStart = HAL_GET_TICK();

        if (HAL_GetTick() - debounceStart > debounceTimeReq) {
            return logicLevelOnPress;
        } else {
            if (logicLevelOnPress == LogicLevelOnPress::HIGH) {
                return LogicLevelOnPress::LOW;
            } else {
                return LogicLevelOnPress::HIGH;
            }
        }

    }


}// namespace EVT::core::DEV