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
     * @param loopbackEnabled Flag to enable CAN loop back functionality
     */
    CAN(Pin txPin, Pin rxPin, bool loopbackEnabled);

    /**
     * Transmit the message over CAN.
     *
     * @param message The message to send over CAN.
     */
    virtual void transmit(CANMessage& message) = 0;

    /**
     * Receive a message over CAN. The user can either receive in blocking or
     * non-blocking mode. In blocking mode, the code will hang until a message
     * is received then return a pointer to the message that was passed in.
     * In non-blocking, a nullptr will be returned if no message is currently
     * in the mailbox.
     *
     * @param message {out} The message to populate with data
     * @param blocking Used to determine if received should block or not, by
     *      default receive is blocking
     * @return A pointer to the passed in message, nullptr if message not
     *      received.
     */
    virtual CANMessage* receive(CANMessage* message, bool blocking=true) = 0;

private:
    /** The CAN transmit pin */
    Pin txPin;
    /** The CAN receive pin */
    Pin rxPin;
    /** Represents if filtering should take place for CAN ids */
    bool filtering;
    /** If CAN should operate in loop back mode */
    bool loopbackEnabled;
};
}  // namespace EVT::core::IO

#endif
