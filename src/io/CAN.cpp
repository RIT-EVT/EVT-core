#include <stdint.h>

#include <EVT/io/CAN.hpp>

namespace EVT::core::IO {

CAN::CAN(Pin txPin, Pin rxPin, bool loopbackEnabled) {
    this->txPin = txPin;
    this->rxPin = rxPin;
    this->loopbackEnabled = loopbackEnabled;
}

}  // namespace EVT::core::IO
