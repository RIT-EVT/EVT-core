#include <stdint.h>

#include <EVT/io/CAN.hpp>

namespace EVT::core::IO {

CAN::CAN(Pin txPin, Pin rxPin, bool loopbackEnabled) :
    txPin(txPin),
    rxPin(rxPin),
    loopbackEnabled(loopbackEnabled) {
}

}  // namespace EVT::core::IO
