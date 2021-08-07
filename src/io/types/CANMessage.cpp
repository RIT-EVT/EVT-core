#include <stdint.h>
#include <cstring>

#include <EVT/io/types/CANMessage.hpp>

namespace EVT::core::IO {

CANMessage::CANMessage(uint32_t id, uint8_t dataLength, uint8_t* payload) {
    this->id = id;
    // TODO: Should include way to notify user of invalid CAN frame,
    // to be added to error manager later.
    this->dataLength = dataLength <= CAN_MAX_PAYLOAD_SIZE ?
        dataLength : CAN_MAX_PAYLOAD_SIZE;

    // Copy contents of provided payload into message's payload
    for (int i = 0; i < this->dataLength; i++)
        this->payload[i] = payload[i];
}

CANMessage::CANMessage() {
    this->id = 0;
    this->dataLength = 0;
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

void CANMessage::setId(uint32_t id) {
    this->id = id;
}

void CANMessage::setDataLength(uint8_t size) {
    this->dataLength = size;
}

void CANMessage::setPayload(uint8_t *payload) {
    std::memcpy(this->payload, payload, CAN_MAX_PAYLOAD_SIZE);
}

}  // namespace EVT::core::IO