#ifndef _EVT_GPIO_
#define _EVT_GPIO_

#include <stdint.h>

namespace EVT::core::IO
{
// Forward declarations:
// The different pins are hardware specific. Forward declaration to allow
// at compilation time the decision of which pins should be used.
enum class Pin;


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
class GPIO
{
public:

    /**
     * Binary representation of the states the GPIO can be in
     */
    enum class State
    {
        LOW     = 0u,
        HIGH    = 1u
    };

    /**
     * Binary representation of the flow of information, either input or
     * output.
     */
    enum class Direction
    {
        INPUT   = 0u,
        OUTPUT  = 1u
    };

    /**
     * Enum to handler the possible GPIO trigger states
     */
    enum class TriggerEdge
    {
        RISING          = 1u,
        FALLING         = 2u,
        RISING_FALLING  = 3u
    };


    /**
     * Create a new GPIO interface on a specific pin. The direction will not
     * be set and will have to be set manually.
     *
     * @param pin[in] The pin for the GPIO instance to use.
     */
    GPIO(Pin pin);

    /**
     * Create a new GPIO instance on a specific pin with a given direction.
     *
     * @param pin[in] The pin for the GPIO instance to use.
     * @param direction[in] The directional flow of data.
     */
    GPIO(Pin pin, Direction direction);

    /**
     * Set the direction of the pin.
     *
     * @param direction[in] The direction of information.
     */
    virtual void setDirection(Direction direction) = 0;

    /**
     * Used for writing a state to a pin.
     *
     * @param state[in] The state to write to the pin
     */
    virtual void writePin(State state) = 0;

    /**
     * Used for reading the state of a pin.
     *
     * @return The state of the pin.
     */
    virtual State readPin() = 0;

    /**
     * Registers the IRQHandler for this instances GPIO pin on the given edge
     * condition.
     * @param edge[in] The edge trigger event to trigger the interrupt
     * @param irqHandler[in] The function pointer to handle the GPIO interrupt
     */
    virtual void registerIRQ(TriggerEdge edge, void (*irqHandler)(GPIO *pin)) = 0;

protected:
    /// The pin the GPIO instance is attached to
    Pin pin;
    /// Direction of the flow of information, input or output
    Direction direction;
};

}  // EVT::core::IO
#endif
