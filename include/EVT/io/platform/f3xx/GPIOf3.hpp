#ifndef _EVT_GPIOf3_
#define _EVT_GPIOf3_

#include <cstdint>

#include <EVT/io/GPIO.hpp>
#include <HALf3/stm32f3xx_hal_gpio.h>


namespace EVT::core::IO
{

class GPIOf3 : public GPIO
{
public:
    
    /**
     * Create an instance of the STMF3xx GPIO pin using the provided pin. The
     * direction will have to be set manually before use.
     *
     * @parm pin The pin for the GPIO instance to use.
     */
    GPIOf3(Pin pin);

    /**
     * Create an instance of the STMF3xx GPIO pin using the provided pin
     * and direction.
     *
     * @param pin The pin for the GPIO instance to use.
     * @param direction The flow of data (either input or output).
     */
    GPIOf3(Pin pin, Direction direction);

    /**
     * Set the direction of the pin.
     *
     * @param direction The direction of information.
     */
    void setDirection(Direction direction);

    /**
     * Used for writing a state to a pin.
     *
     * @param state The sate to write to the pin.
     */
    void writePin(State state);

    /**
     * Used for reading the state of the pin.
     *
     * @return The state of the pin.
     */
    State readPin();
   
private:
    constexpr static uint32_t GPIO_TRIGGER_INTERRUPT_BASE = 0x10010000U;

    /// Pin representing the underlying HAL pin identifer
    uint16_t halPin;
    /// Represents the GPIO port of the pin (A, B, C, D, E, or F)
    GPIO_TypeDef * port;
};
    
}

#endif
