#ifndef _EVT_MANAGER_H_
#define _EVT_MANAGER_H_

#include <stdint.h>

#include <EVT/io/ADC.hpp>
#include <EVT/io/CAN.hpp>
#include <EVT/io/GPIO.hpp>
#include <EVT/io/PWM.hpp>

#ifdef STM32F3xx
    #include <EVT/platform/f3xx/stm32f3xx.hpp>

    #include <EVT/io/platform/f3xx/ADCf3xx.hpp>
    #include <EVT/io/platform/f3xx/CANf3xx.hpp>
    #include <EVT/io/platform/f3xx/GPIOf3xx.hpp>
    #include <EVT/io/platform/f3xx/I2Cf3xx.hpp>
    #include <EVT/io/platform/f3xx/PWMf3xx.hpp>
    #include <EVT/io/platform/f3xx/SPIf3xx.hpp>
    #include <EVT/io/platform/f3xx/UARTf3xx.hpp>
#endif

namespace EVT::core::IO {

/**
 * Initialize the low level components of the system. This is highly
 * platform specific and usually involves clock setup and other peripheral
 * init logic.
 */
void init() {
#ifdef STM32F3xx
    EVT::core::platform::stm32f3xx_init();
#endif
}

/**
 * Get an instance of an ADC channel
 *
 * @param[in] pin The pin to use with the ADC
 */
template<Pin pin>
ADC& getADC() {
#ifdef STM32F3xx
    static ADCf3xx adc(pin);
    return adc;
#endif
}

/**
 * Get an instance of a CAN interface.
 *
 * @param[in] txPin CAN pin for transmitting messages
 * @param[in] rxPin CAN pin for receiving messages
 */
template<Pin txPin, Pin rxPin>
CAN& getCAN(bool loopbackEnabled = false) {
#ifdef STM32F3xx
    static CANf3xx can(txPin, rxPin, loopbackEnabled);
    return can;
#endif
}

/**
 * Get an instance of a GPIO pin.
 *
 * @param[in] pin The pin to attach to the GPIO
 * @param[in] direction The direction, either input or output
 */
template<Pin pin>
GPIO& getGPIO(GPIO::Direction direction = GPIO::Direction::OUTPUT) {
#ifdef STM32F3xx
    static GPIOf3xx gpioPin(pin, direction);
    return gpioPin;
#endif
}

/**
 * Get an I2C master interface.
 *
 * @param[in] scl The I2C clock pin
 * @param[in] sda The I2C data pin
 */
template<Pin scl, Pin sda>
I2C& getI2C() {
#ifdef STM32F3xx
    static I2Cf3xx i2c(scl, sda);
    return i2c;
#endif
}

/**
 * Get an instance of a PWM pin.
 *
 * @param[in] pin The pin to attach to the PWM.
 */
template<Pin pin>
PWM& getPWM() {
#ifdef STM32F3xx
    static PWMf3xx pwm(pin);
    return pwm;
#endif
}

/*
 * Get an instance of a UART.
 *
 * @param[in] txPin The transmit pin for the UART.
 * @param[in] rxPin The receive pin for the UART.
 * @param[in] baudrate The baudrate to operate at..
 */
template<Pin txPin, Pin rxPin>
UART& getUART(uint32_t baudrate) {
#ifdef STM32F3xx
    static UARTf3xx uart(txPin, rxPin, baudrate);
    return uart;
#endif
}

template<Pin sckPin, Pin mosiPin, Pin misoPin>
SPI& getSPI(GPIO* CSPins[], uint8_t pinLength) {
#ifdef STM32F3xx
    static SPIf3xx spi(CSPins, pinLength, sckPin, mosiPin, misoPin);
    return spi;
#endif
}

template<Pin sckPin, Pin mosiPin>
    SPI& getSPI(GPIO* CSPins[], uint8_t pinLength) {
#ifdef STM32F3xx
    static SPIf3xx spi(CSPins, pinLength, sckPin, mosiPin);
    return spi;
#endif
}

}// namespace EVT::core::IO

#endif
