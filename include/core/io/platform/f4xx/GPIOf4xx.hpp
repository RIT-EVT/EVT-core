#ifndef _EVT_GPIOf4xx_
#define _EVT_GPIOf4xx_

#include <cstdint>

#include <HALf4/stm32f4xx.h>
#include <core/io/GPIO.hpp>

namespace EVT::core::IO {

class GPIOf4xx : public GPIO {
public:
    /**
     * Create an instance of the STMF4xx GPIO pin using the provided pin. The
     * direction will have to be set manually before use.
     *
     * @param[in] pin The pin for the GPIO instance to use.
     */
    GPIOf4xx(Pin pin);

    /**
     * Create an instance of the STMF4xx GPIO pin using the provided pin
     * and direction.
     *
     * @param[in] pin The pin for the GPIO instance to use.
     * @param[in] direction The flow of data (either input or output).
     */
    GPIOf4xx(Pin pin, Direction direction, Pull pull = Pull::PULL_DOWN);

    void setDirection(Direction direction) override;

    void writePin(State state) override;

    State readPin() override;

    void registerIRQ(TriggerEdge edge, void (*irqHandler)(GPIO* pin, void* priv), void* priv) override;

    /**
     * Condenses gpio settings initialization into a single function.
     * @param targetGpio gpio instance to initialize
     * @param pins array of pins used by gpio instance
     * @param numOfPins size of the pin array (either 1 or 2)
     * @param mode gpio configuration mode
     * @param pull pull-up or pull-down activation
     * @param speed maximum gpio output frequency
     * Possible values for Mode, Pull, and Speed can be found in "stm32f4xx_hal_gpio.h"
     * @param alternate gpio alternate function selection
     */
    static void gpioStateInit(GPIO_InitTypeDef* targetGpio, Pin* pins,
                              uint8_t numOfPins, uint32_t mode, uint32_t pull,
                              uint32_t speed, uint8_t alternate = 0x0DU);

private:
    // See stm32f4xx_hal_gpio -> GPIO_mode for info on derivations
    constexpr static int GPIO_MODE_IT_SHIFT = 20;
    constexpr static uint32_t GPIO_TRIGGER_INTERRUPT_BASE = GPIO_MODE_IT_RISING & ~(1 << GPIO_MODE_IT_SHIFT);

    /// Pin representing the underlying HAL pin identifer
    uint16_t halPin;
    /// Represents the GPIO port of the pin (A, B, C, D, E, or F)
    GPIO_TypeDef* port;
};

}// namespace EVT::core::IO

#endif
