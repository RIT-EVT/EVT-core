#include <stdint.h>

#include <EVT/io/CAN.hpp>

namespace EVT::core::IO {

CAN::CAN(Pin txPin, Pin rxPin, uint8_t* CANids, uint8_t numCANids) {
    this->txPin = txPin;
    this->rxPin = rxPin;
    this->CANids = CANids;
    this->numCANids = numCANids;
    this->filtering = numCANids > 0;
}

}  // namespace EVT::core::IO
