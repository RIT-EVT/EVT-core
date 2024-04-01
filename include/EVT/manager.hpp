#ifndef _EVT_MANAGER_H_
#define _EVT_MANAGER_H_

#include <cstdint>

#include <EVT/io/ADC.hpp>
#include <EVT/io/CAN.hpp>
#include <EVT/io/GPIO.hpp>
#include <EVT/io/I2C.hpp>
#include <EVT/io/PWM.hpp>
#include <EVT/io/UART.hpp>
#include <EVT/io/pin.hpp>

#ifdef STM32F3xx
    #define IWDG_SUPPORTED
    #define RTC_SUPPORTED
    #define MCU_SUPPORTED
    #define ADC_SUPPORTED
    #define GPIO_SUPPORTED
    #define I2C_SUPPORTED
    #define PWM_SUPPORTED
    #define UART_SUPPORTED
    #define SPI_SUPPORTED
    #define CAN_SUPPORTED

    #include <EVT/dev/MCUTimer.hpp>
    #include <EVT/dev/platform/f3xx/IWDGf3xx.hpp>
    #include <EVT/dev/platform/f3xx/RTCf3xx.hpp>
    #include <EVT/dev/platform/f3xx/Timerf3xx.hpp>
    #include <EVT/io/platform/f3xx/ADCf3xx.hpp>
    #include <EVT/io/platform/f3xx/CANf3xx.hpp>
    #include <EVT/io/platform/f3xx/GPIOf3xx.hpp>
    #include <EVT/io/platform/f3xx/I2Cf3xx.hpp>
    #include <EVT/io/platform/f3xx/PWMf3xx.hpp>
    #include <EVT/io/platform/f3xx/SPIf3xx.hpp>
    #include <EVT/io/platform/f3xx/UARTf3xx.hpp>
    #include <EVT/platform/f3xx/stm32f3xx.hpp>
#endif

#ifdef STM32F4xx
    #define GPIO_SUPPORTED
    #define UART_SUPPORTED
    #define PWM_SUPPORTED
    #define SPI_SUPPORTED
    #define ADC_SUPPORTED

    #include <EVT/io/platform/f4xx/ADCf4xx.hpp>
    #include <EVT/platform/f4xx/stm32f4xx.hpp>
    //    #include <EVT/io/platform/f4xx/CANf4xx.hpp>
    #include <EVT/dev/platform/f4xx/IWDGf4xx.hpp>
    #include <EVT/io/platform/f4xx/GPIOf4xx.hpp>
    #include <EVT/io/platform/f4xx/I2Cf4xx.hpp>
    #include <EVT/io/platform/f4xx/PWMf4xx.hpp>
    #include <EVT/io/platform/f4xx/SPIf4xx.hpp>
    #include <EVT/io/platform/f4xx/UARTf4xx.hpp>
#endif

namespace EVT::core::platform {

/**
* Initialize the low level components of the system. This is highly
* platform specific and usually involves clock setup and other peripheral
* init logic.
*/
void init() {
#ifdef STM32F3xx
    stm32f3xx_init();
#endif
#ifdef STM32F4xx
    EVT::core::platform::stm32f4xx_init();
#endif
}

}// namespace EVT::core::platform

namespace EVT::core::DEV {

/**
* Get an instance of an IWDG
*
* @param ms Time in milliseconds before the IWDG triggers a reset
* must be a value between 8 and 32768 ms.
*/
#ifdef IWDG_SUPPORTED
IWDG& getIWDG(uint32_t ms) {
    #ifdef STM32F3xx
    // 8 < ms < 32768
    static IWDGf3xx iwdg(ms);
    return iwdg;
    #endif
    #ifdef STM32F4xx
    // 8 < ms < 32768
    // Found in IWDG Main Features:
    // https://www.st.com/resource/en/reference_manual/dm00135183-stm32f446xx-advanced-arm-based-32-bit-mcus-stmicroelectronics.pdf

    static IWDGf4xx iwdg(ms);
    return iwdg;
    #endif
}
#endif

/**
* Get an instance of an RTC
*/
#ifdef RTC_SUPPORTED
RTC& getRTC() {
    #ifdef STM32F3xx
    static RTCf3xx rtc;
    return rtc;
    #endif
}
#endif

#ifdef MCU_SUPPORTED
template<MCUTimer mcuTimer>
Timer& getTimer(uint32_t clockPeriod) {
    #ifdef STM32F3xx
    static Timerf3xx timer(getTIM(mcuTimer), clockPeriod);
    return timer;
    #endif
}
#endif

}// namespace EVT::core::DEV

namespace EVT::core::IO {

/**
* Get an instance of an ADC channel
*
* @param[in] pin The pin to use with the ADC
*/
#ifdef ADC_SUPPORTED
template<Pin pin>
ADC& getADC() {
    #ifdef STM32F4xx
    static ADCf4xx adc(pin);
    return adc;
    #endif
    #ifdef STM32F3xx
    static ADCf3xx adc(pin);
    return adc;
    #endif
}
#endif

/**
* Get an instance of a CAN interface.
*
* @param[in] txPin CAN pin for transmitting messages
* @param[in] rxPin CAN pin for receiving messages
*/
#ifdef CAN_SUPPORTED
template<Pin txPin, Pin rxPin>
CAN& getCAN(bool loopbackEnabled = false) {
    #ifdef STM32F3xx
    static CANf3xx can(txPin, rxPin, loopbackEnabled);
    return can;
    #endif
    #ifdef STM32F4xx
    static CANf4xx can(txPin, rxPin, loopbackEnabled);
    return can;
    #endif
}
#endif

/**
* Get an instance of a GPIO pin.
*
* @param[in] pin The pin to attach to the GPIO
* @param[in] direction The direction, either input or output
* @param[in] pull The direction of the internal pull resistor
*/
#ifdef GPIO_SUPPORTED
template<Pin pin>
GPIO& getGPIO(GPIO::Direction direction = GPIO::Direction::OUTPUT,
              GPIO::Pull pull = GPIO::Pull::PULL_DOWN) {
    #ifdef STM32F3xx
    static GPIOf3xx gpioPin(pin, direction, pull);
    return gpioPin;
    #endif
    #ifdef STM32F4xx
    static GPIOf4xx gpioPin(pin, direction);
    return gpioPin;
    #endif
}
#endif

/**
* Get an I2C master interface.
*
* @param[in] scl The I2C clock pin
* @param[in] sda The I2C data pin
*/
#ifdef I2C_SUPPORTED
template<Pin scl, Pin sda>
I2C& getI2C() {
    #ifdef STM32F3xx
    static I2Cf3xx i2c(scl, sda);
    return i2c;
    #endif
    #ifdef STM32F4xx
    static I2Cf4xx i2c(scl, sda);
    return i2c;
    #endif
}
#endif

/**
* Get an instance of a PWM pin.
*
* @param[in] pin The pin to attach to the PWM.
*/
#ifdef PWM_SUPPORTED
template<Pin pin>
PWM& getPWM() {
    #ifdef STM32F3xx
    static PWMf3xx pwm(pin);
    return pwm;
    #endif
    #ifdef STM32F4xx
    static PWMf4xx pwm(pin);
    return pwm;
    #endif
}
#endif

/**
* Get an instance of a UART.
*
* @param[in] txPin The transmit pin for the UART
* @param[in] rxPin The receive pin for the UART
* @param[in] baudrate The baudrate to operate at
* @param[in] isSwapped Whether TX and RX should be swapped; defaults to false
 */
#ifdef UART_SUPPORTED
template<Pin txPin, Pin rxPin>
UART& getUART(uint32_t baudrate, bool isSwapped = false) {
    #ifdef STM32F3xx
    static UARTf3xx uart(txPin, rxPin, baudrate, isSwapped);
    return uart;
    #endif
    #ifdef STM32F4xx
    static UARTf4xx uart(txPin, rxPin, baudrate, isSwapped);
    return uart;
    #endif
}
#endif

/**
* Get an instance of a SPI driver.
*
* @tparam sckPin Serial clock pin
* @tparam mosiPin Master out, slave in pin
* @tparam misoPin Master in, slave out pin
* @param CSPins Array of chip select pins
* @param pinLength Number of chip select pins in the array
*/
#ifdef SPI_SUPPORTED
template<Pin sckPin, Pin mosiPin, Pin misoPin>
SPI& getSPI(GPIO* CSPins[], uint8_t pinLength) {
    #ifdef STM32F3xx
    static SPIf3xx spi(CSPins, pinLength, sckPin, mosiPin, misoPin);
    return spi;
    #endif
    #ifdef STM32F4xx
    static SPIf4xx spi(CSPins, pinLength, sckPin, mosiPin, misoPin);
    return spi;
#endif
}

/**
* Get an instance of a write-only SPI driver.
*
* @tparam sckPin Serial clock pin
* @tparam mosiPin Master out, slave in pin
* @param CSPins Array of chip select pins
* @param pinLength Number of chip select pins in the array
*/
template<Pin sckPin, Pin mosiPin>
SPI& getSPI(GPIO* CSPins[], uint8_t pinLength) {
    #ifdef STM32F3xx
    static SPIf3xx spi(CSPins, pinLength, sckPin, mosiPin);
    return spi;
    #endif
}
#endif

}// namespace EVT::core::IO

#endif
