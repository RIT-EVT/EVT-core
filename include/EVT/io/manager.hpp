#ifndef _EVT_MANAGER_H_
#define _EVT_MANAGER_H_

#include <stdint.h>

#include <EVT/io/GPIO.hpp>

#ifdef STM32F302x8
    #include <EVT/io/platform/f3xx/f302x8/GPIOf302x8.hpp>
    #include <EVT/io/platform/f3xx/f302x8/UARTf302x8.hpp>
#endif

namespace EVT::core::IO
{

/**
 * Get an instance of a GPIO pin.
 *
 * @param pin The pin to attach to the GPIO
 * @param direction The direction, either input or output
 */
template<Pin pin>
GPIO& getGPIO(GPIO::Direction direction=GPIO::Direction::OUTPUT) {
    #ifdef STM32F302x8
        static GPIOf302x8 gpioPin(pin, direction);
        return gpioPin;
    #endif
}

/**
 * Get an instance of a UART.
 *
 * @param txPin The transmit pin for the UART.
 * @param rxPin The receive pin for the UART.
 * @param baudrate The baudrate to operate at..
 */
template<Pin txPin, Pin rxPin>
UART& getUART(uint32_t baudrate) {
    #ifdef STM32F302x8
        static UARTf302x8 uart(txPin, rxPin, baudrate);
        return uart;
    #endif
}

}  // EVT::core::IO
#endif
