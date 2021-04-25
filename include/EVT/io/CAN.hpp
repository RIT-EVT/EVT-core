#ifndef _EVT_CAN_
#define _EVT_CAN_

#include <stdint.h>

#include <EVT/io/types/CANMessage.hpp>

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
