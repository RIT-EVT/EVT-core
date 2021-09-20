#ifndef _EVT_CANMESSAGE_
#define _EVT_CANMESSAGE_

#include <stdint.h>

namespace EVT::core::IO
{

/**
 * Represents a generic CAN message which can be sent and received
 * over a CAN bus.
 */
class CANMessage
{
public:

    /** Represents the maximum payload size of a CAN message */
    static constexpr uint8_t CAN_MAX_PAYLOAD_SIZE = 8;

    /**
     * Create an instance of a CANMessage
     *
     * @param id[in] The id of the CAN Message
     * @param dataLength[in] The size of the payload (<= CAN_MAX_PAYLOAD_SIZE)
     * @param payload[in] The data in the CAN message
     */
    CANMessage(uint32_t id, uint8_t dataLength, uint8_t* payload);

    /**
     * Create an empty CANMessage, the id will be 0, dataLength will be 0 and
     * the payload will be empty
     */
    CANMessage();

    /**
     * Get the id of the CAN message
     *
     * @return id of the specific CAN message
     */
    uint32_t getId();

    /**
     * Get the size of the payload in bytes.
     *
     * @return The size of the payload in bytes.
     */
    uint8_t getDataLength();

    /**
     * Get the pointer to the payload.
     *
     * @return pointer to the payload.
     */
    uint8_t* getPayload();

    /**
     * Set the id of the CAN Message
     *
     * @param id[in] The id of the CAN message
     */
    void setId(uint32_t id);

    /**
     * Set the size of the payload in bytes
     *
     * @param size[in] The size of the payload in bytes
     */
    void setDataLength(uint8_t size);

    /**
     * Set the payload to the contents of the provided array. Does a copy
     * of the bytes stored.
     *
     * NOTE: Will copy over CAN_MAX_PAYLOAD_SIZE number of bytes over
     *
     * @param payload[in] The payload to copy over
     */
    void setPayload(const uint8_t* payload);
    
    /**
     * Assignment operator where the contents of the CANMessage is copied into
     * this message.
     * @param other The CANMessage to copy from
     */
    CANMessage& operator= (const CANMessage& other);

private:
    /** ID of the CAN message */
    uint32_t id;
    /** Number of data bytes that are being used  (must be at most 8 bytes) */
    uint8_t dataLength;
    /** The data stored in the CAN message */
    uint8_t payload[CAN_MAX_PAYLOAD_SIZE];
};

}  // namespace EVT::core::IO


#endif
