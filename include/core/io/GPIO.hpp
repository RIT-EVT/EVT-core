#ifndef _EVT_GPIO_
#define _EVT_GPIO_

#include "pin.hpp"

#include <stdint.h>

namespace core::io {
// Forward declarations:
// The different pins are hardware specific. Forward declaration to allow
// at compilation time the decision of which pins should be used.
enum class Pin  : uint8_t;
enum class Port : uint8_t;

/**
 * Interface for interacting with GPIO pins on a device. GPIO pins can have
 * their state read and written to.
 *
 * This provides the standard set of features supported across all hardware
 * for GPIO pins. Hardware specific features are not included. This interface
 * should be used for the majority of cases and should only be deviated from
 * when there is a need for hardware specific functionality.
 *
 * NOTE: You cannot directly make an instance of this class via a constructor.
 * To make an instance, use the GPIO::getInstance method.
 */
class GPIO {
public:
    /**
     * A union of a 16-bit number and a  breakdown of it in a struct of individual bits.
     * the value is meant to quickly set or read a value with ease.
     * the struct is meant for individually flipping bits in the field with a friendly format
     * Meant for mass GPIO instantiation
     *
     * Usage Example 1:
     * PinPack a = {.value = 0x0010};
     * PinPack b = {.pin_4 = 1};
     * then
     * a == b
     *
     * Usage Example 2:
     * PinPack a = {.pin_4 = 1, .pin_15 = 1};
     * then reading from a.value gives
     * a.value == 0x8010;
     *
     * Usage Example 3:
     * uint16_t any_number = 0x1111;
     * PinPack a;
     * a.value = any_number;
     * then
     * a.pin_12 && a.pin_8 && a.pin_4 && a.pin_0 == 1
     */
    #pragma pack(push, 1)
    union PinPack {
        uint16_t value;
        struct {
            uint16_t pin_0 : 1, // Bit 0
                pin_1 : 1, pin_2 : 1, pin_3 : 1, pin_4 : 1, pin_5 : 1, pin_6 : 1, pin_7 : 1, pin_8 : 1, pin_9 : 1,
                pin_10 : 1, pin_11 : 1, pin_12 : 1, pin_13 : 1, pin_14 : 1,
                pin_15 : 1; // Bit 15
        };
    };

    /**
     * Binary representation of the states the GPIO can be in
     */
    enum class State {
        LOW  = 0u,
        HIGH = 1u
    };

    /**
     * Binary representation of the flow of information, either input or
     * output
     */
    enum class Direction {
        INPUT  = 0u,
        OUTPUT = 1u
    };

    /**
     * Enum to handler the possible GPIO trigger states
     */
    enum class TriggerEdge {
        RISING         = 1u,
        FALLING        = 2u,
        RISING_FALLING = 3u
    };

    /**
     * Direction for the internal resistor
     */
    enum class Pull {
        NO_PULL   = 0u,
        PULL_UP   = 1u,
        PULL_DOWN = 2u,
    };

    /**
     * Create a new GPIO interface on a specific pin. The direction will not
     * be set and will have to be set manually.
     *
     * @param[in] pin The pin for the GPIO instance to use.
     */
    GPIO(Pin pin);

    /**
     * Create a new GPIO instance on a specific pin with a given direction.
     *
     * @param[in] pin The pin for the GPIO instance to use.
     * @param[in] direction The directional flow of data.
     * @param[in] pull The direction of the internal pull resistor
     */
    GPIO(Pin pin, Direction direction, Pull pull = Pull::PULL_DOWN);

    /**
     *
     * @param pin
     * @return uint16_t with the pin number's bit set
     */
    static uint16_t setPackBit(Pin pin) {
        return 1 << pinNumberFromPin(pin);
    }

    /**
     * Fill a given PinPack based on a given array of pins
     *
     * @param[out] pp PinPack to be filled given the pin array
     * @param[in] pins Array of pins in the same Port
     * @param[in] num_pins number of pins in array
     */
    static void fillPinPack(PinPack& pp, Pin* pins, uint8_t num_pins) {
        pp.value = 0;
        for (uint8_t i = 0; i < num_pins; i++) {
            pp.value |= setPackBit(pins[i]);
        }
    }

    /**
     * Sets whether this pin is configured for input or output.
     *
     * @param[in] direction Pin direction: Direction::Input or Direction::Output.
     */
    virtual void setDirection(Direction direction) = 0;

    /**
     * Set the output of this pin to the given logic state.
     *
     * @param[in] state Logic level to set: State::LOW or State::HIGH.
     */
    virtual void writePin(State state) = 0;

    /**
     * Read the current logic state of this pin.
     *
     * @return The pin's logic level: State::LOW or State::HIGH.
     */
    virtual State readPin() = 0;

    /**
     * Registers the IRQHandler for this instances GPIO pin on the given edge
     * condition.
     * @param[in] edge The edge trigger event to trigger the interrupt
     * @param[in] irqHandler The function pointer to handle the GPIO interrupt
     * @param[in] priv private data the function may use
     */
    virtual void registerIRQ(TriggerEdge edge, void (*irqHandler)(GPIO* pin, void* priv), void* priv) = 0;

protected:
    /// The pin the GPIO instance is attached to
    Pin pin;
    /// Direction of the flow of information, input or output
    Direction direction;
    /// The direction the pin is pulled internally
    Pull pull;
};

} // namespace core::io
#endif
