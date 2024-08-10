#ifndef _EVT_LED_
#define _EVT_LED_

#include <core/io/GPIO.hpp>

namespace EVT::core::DEV {

class LED {
public:
    /**
     * Represents if the LED is active high or active low.
     */
    enum class ActiveState {
        HIGH = 0u,
        LOW = 1u
    };

    /**
     * Create an instance of the LED based on the given GPIO pin.
     *
     * @param[in] gpio GPIO pin
     * @param[in] activeState Represents if the LED is active high or active low
     */
    LED(EVT::core::IO::GPIO& gpio, ActiveState activeState);

    /**
     * Toggle the current state of the LED
     */
    void toggle();

    /**
     * Set the current state of the LED.
     *
     * @param[in] state The state to set the LED to.
     */
    void setState(EVT::core::IO::GPIO::State state);

private:
    /// The gpio pin used by the LED
    EVT::core::IO::GPIO& gpio;
    /// If the LED is active high or active low
    ActiveState activeState;
};

}// namespace EVT::core::DEV

#endif
