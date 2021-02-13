#include <EVT/io/GPIO.hpp>


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

}         
