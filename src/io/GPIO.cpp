#include <EVT/io/GPIO.hpp>

// Find out which platform being built for
#ifdef STM32F302x8
    #include <EVT/io/platform/f3xx/f302x8/GPIOf302x8.hpp>
#else
    #error "The target platform does not support GPIO"
#endif

namespace EVT::core::IO
{

// Setup instance variables
GPIO::GPIO(Pin pin) 
{
    this->pin = pin;
}

GPIO::GPIO(Pin pin, Direction direction) 
{
    this->pin = pin;
    this->direction = direction;
}

GPIO& GPIO::getInstance(Pin pin, Direction direction)
{
    // Determine which GPIO interface to use
    #ifdef STM32F302x8
        static GPIOf302x8 gpioPin(pin, direction);
        return gpioPin;
    #endif
}

}         
