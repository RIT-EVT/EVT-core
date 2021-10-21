#ifndef _EVT_GPIOf302x8_
#define _EVT_GPIOf302x8_

#include <cstdint>

#include <EVT/io/GPIO.hpp>
#include <HALf3/stm32f3xx.h>


namespace EVT::core::IO
{

class GPIOf302x8 : public GPIO
{
public:
    
    /**
     * Create an instance of the STMF3xx GPIO pin using the provided pin. The
     * direction will have to be set manually before use.
     *
     * @parm pin The pin for the GPIO instance to use.
     */
    GPIOf302x8(Pin pin);

    /**
     * Create an instance of the STMF3xx GPIO pin using the provided pin
     * and direction.
     *
     * @param pin The pin for the GPIO instance to use.
     * @param direction The flow of data (either input or output).
     */
    GPIOf302x8(Pin pin, Direction direction);

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

    /**
     * Used to register an IRQ Handler for the GPIO instance
     * @param edge the trigger edge
     * @param irqHandler the function pointer to the handler
     */
    void registerIRQ(TriggerEdge edge, void (*irqHandler)(GPIO *pin)) override;

    /**
     * Condenses gpio settings initialization into a single function.
     * @param targetGpio gpio instance to initialize
     * @param pins array of pins used by gpio instance
     * @param mode gpio configuration mode
     * @param pull pull-up or pull-down activation
     * @param speed maximum gpio output frequency
     * @param alternate gpio alternate function selection
     */
    static void gpioStateInit(GPIO_InitTypeDef *targetGpio, Pin *pins, uint32_t mode, 
        uint32_t pull, uint32_t speed, uint8_t alternate = 0x0DU);
   
private:
    // See stm32f3xx_hal_gpio -> GPIO_mode for info on derivations
    constexpr static int GPIO_MODE_IT_SHIFT = 20;
    constexpr static uint32_t GPIO_TRIGGER_INTERRUPT_BASE = GPIO_MODE_IT_RISING & ~(1 << GPIO_MODE_IT_SHIFT);

    /// Pin representing the underlying HAL pin identifer
    uint16_t halPin;
    /// Represents the GPIO port of the pin (A, B, C, D, E, or F)
    GPIO_TypeDef * port;
};
    
}

#endif
