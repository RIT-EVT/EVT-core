#ifndef _EVT_GPIO_
#define _EVT_GPIO_

#include <stdint.h>

namespace EVT::core::IO
{
// Forward declarations:
// The different pins are hardware specific. Forward declaration to allow
// at compilation time the decision of which pins should be used.
enum class Pin;

/**
 * Generic interface for CAN bus communication. Devices can send and recieve
 * messages over the CAN bus using this interface.
 *
 * This provides the standard set of features supported across all hardware
 * for CAN communication. Any hardware specific features are not included.
 * This interface should be used for the majority of cases and should be
 * deviated from when there is a need for hardware spoecific functionality.
 *
 * NOTE: You cannot directly make an instance of this class via a constructor.
 * You will need to use the getCAN function in the IO namespace.
 */
class CAN
{
public:

    /**
     * Represents the format of all CAN messages used for sending and
     * receiving CAN messages over the bus.
     */
    struct CANMessage
    {
        /** ID of the CAN message */
        uint32_t id;
        /** Number of bytes used for the data (<=MAX_CAN_MESSAGE_LENGTH) */
        uint8_t dataLength;
        /** The data stored in the CAN message */
        uint8_t* payload;

        CANMessage(uint32_t id, uint8_t* payload, uint8_t dataLength) {
            this->id = id;
            this->dataLength = dataLength;
            // Copy over payload content
            for(uint8_t i = 0; i < dataLength; i++)
                this->payload[i] = payload[i];
        }
    };


    /**
     * Creates a new instance of the CAN interface which will use the given
     * transmit and receive pins.
     *
     * @param txPin The pin to use for transmitting data
     * @param rxPin The pin to use for receiving data
     * @param CANids Array of the IDs that will be recognized
     * @param numCANids Number of CANids that were passed in
     */
    CAN(Pin txPin, Pin rxPin, uint8_t* CANids, uint8_t numCANids);

    /**
     * Transmit the message over CAN.
     *
     * @param message The message to send over CAN.
     */
    virtual void transmit(CANMessage message) = 0;

    /**
     * Receive a message with the given ID. If a message with that ID has
     * not been received, then nullptr is returned.
     *
     * @param message {out} The message to populate with data
     * @return A pointer to the passed in message, nullptr if message not
     *      received.
     */
    virtual CANMessage* receive(CANMessage* message) = 0;

    /**
     * Add a call back that will be called when a CAN message of a specific
     * ID is received.
     *
     * NOTE: If you add a call back for the given ID, then the message will
     *      not be added to the receive buffer (cannot later call
     *      CAN::receive and expect the message to be there)
     *
     * @param callback Function to call when the message is recieved
     * @param id The CAN message to attach the call back to
     */
    virtual void addCallback(void (*callback)(CANMessage* message),
                             uint8_t id);


    /** Maximum size any CAN message can be in bytes */
    constexpr static uint8_t MAX_CAN_MESSAGE_LENGTH = 8;

private:
    /** CAN ids that will be filtered and received */
    uint8_t* CANids;
    /** Number of CAN ids that are recognized */
    uint8_t numCANids;
};
}  // namespace EVT::core::IO

#endif
