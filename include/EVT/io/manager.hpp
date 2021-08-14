#ifndef _EVT_MANAGER_H_
#define _EVT_MANAGER_H_

#include <stdint.h>

#include <EVT/io/ADC.hpp>
#include <EVT/io/CAN.hpp>
#include <EVT/io/GPIO.hpp>
#include <EVT/io/PWM.hpp>

#ifdef STM32F302x8
    #include <EVT/platform/f3xx/stm32f302x8.hpp>

    #include <EVT/io/platform/f3xx/f302x8/ADCf302x8.hpp>
    #include <EVT/io/platform/f3xx/f302x8/CANf302x8.hpp>
    #include <EVT/io/platform/f3xx/f302x8/GPIOf302x8.hpp>
    #include <EVT/io/platform/f3xx/f302x8/I2Cf302x8.hpp>
    #include <EVT/io/platform/f3xx/f302x8/PWMf302x8.hpp>
    #include <EVT/io/platform/f3xx/f302x8/UARTf302x8.hpp>
#endif

namespace EVT::core::IO
{

/**
 * Initialize the low level components of the system. This is highly
 * platform specific and usually involves clock setup and other peripheral
 * init logic.
 */
void init() {
    #ifdef STM32F302x8
    EVT::core::platform::stm32f302x8_init();
    #endif
}

/**
 * Get an instance of an ADC channel
 *
 * @param pin The pin to use with the ADC
 */
template<Pin pin>
ADC& getADC() {
    #ifdef STM32F302x8
        static ADCf302x8 adc(pin);
        return adc;
    #endif
}

/**
 * Get an instance of a CAN interface.
 *
 * @param txPin CAN pin for transmitting messages
 * @param rxPin CAN pin for receiving messages
 */
template<Pin txPin, Pin rxPin>
CAN& getCAN(bool loopbackEnabled=false) {
    #ifdef STM32F302x8
        static CANf302x8 can(txPin, rxPin, loopbackEnabled);
        return can;
    #endif
}

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
 * Get an I2C master interface.
 *
 * @param scl The I2C clock pin
 * @param sda The I2C data pin
 */
template<Pin scl, Pin sda>
I2C& getI2C() {
    #ifdef STM32F302x8
        static I2Cf302x8 i2c(scl, sda);
        return i2c;
    #endif
}

/**
 * Get an instance of a PWM pin.
 *
 * @param pin The pin to attach to the PWM.
 */
template<Pin pin>
PWM& getPWM() {
    #ifdef STM32F302x8
        static PWMf302x8 pwm(pin);
        return pwm;
    #endif
}

/*
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
