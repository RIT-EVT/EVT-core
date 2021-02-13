#include <EVT/io/GPIO.hpp>

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

// Create an instance of the GPIO based on the hardware selected
template<Pin pin, Direction direction>
GPIO& GPIO::getInstance()
{
    #ifdef STM32F302x8
        #include <EVT/io/platform/f3xx/GPIOf3.hpp>
        
        static GPIOf3 gpioInstance(pin, direction);

        return gpioInstance;

    #else
        #error "Either the hardware is not supported or not set correctly"
    #endif
}
               
