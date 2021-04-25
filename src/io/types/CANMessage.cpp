#include <stdint.h>

#include <EVT/io/types/CANMessage.hpp>

namespace EVT::core::IO
{

CANMessage::CANMessage(uint32_t id, uint8_t dataLength, uint8_t* payload) {
    this->id = id;
    this->dataLength = dataLength;

    // Copy contents of provided payload into message's payload
    for(int i = 0; i < dataLength; i++)
        this->payload[i] = payload[i];
}

uint32_t CANMessage::getId() {
    return id;
}

uint8_t CANMessage::getDataLength() {
    return dataLength;
}

uint8_t* CANMessage::getPayload() {
    return &payload[0];
}

}  // namespace EVT::core::IO
