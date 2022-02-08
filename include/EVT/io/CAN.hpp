#ifndef _EVT_CAN_
#define _EVT_CAN_

#include <stdint.h>

#include <EVT/io/types/CANMessage.hpp>

namespace EVT::core::IO {
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
class CAN {
public:
    /**
     * Represents potential errors that may take place when using the CAN
     * interface.
     */
    enum class CANStatus {
        TIMEOUT = 0,
        ERROR = 1,
        OK = 3
    };

    /**
     * Creates a new instance of the CAN interface which will use the given
     * transmit and receive pins.
     *
     * @param[in] txPin The pin to use for transmitting data
     * @param[in] rxPin The pin to use for receiving data
     * @param[in] loopbackEnabled Flag to enable CAN loop back functionality
     */
    CAN(Pin txPin, Pin rxPin, bool loopbackEnabled = false);

    /**
     * Transmit the message over CAN.
     *
     * @param[in] message The message to send over CAN.
     * @return The status associated with sending the message
     */
    virtual CANStatus transmit(CANMessage& message) = 0;

    /**
     * Receive a message over CAN. The user can either receive in blocking or
     * non-blocking mode. In blocking mode, the code will hang until a message
     * is received then return a pointer to the message that was passed in.
     * In non-blocking, a nullptr will be returned if no message is currently
     * in the mailbox.
     *
     * @param[out] message The message to populate with data
     * @param[in] blocking Used to determine if received should block or not, by
     *      default receive is blocking
     * @return The status of the receive call, CANStatus::TIMEOUT returned if
     *      no message received
     */
    virtual CANStatus receive(CANMessage* message, bool blocking = false) = 0;

    /**
     * Add an interrupt handler for CAN messages. This will be called with
     * the provided private data when a new CAN message comes in.
     *
     * NOTE: Having an interrupt handler will bypass the CAN's internal
     * storage queue thus making CAN::receive ineffective.
     *
     * @param[in] handler The interrupt handler. Takes in a CANmessage and some other parameter
     * @param[in] priv The private data to pass into the handler
     */
    void addIRQHandler(void (*handler)(CANMessage&, void* priv), void* priv);

    /**
     * Default CAN baudrate.
     */
    static constexpr uint32_t DEFAULT_BAUD = 500000;

private:
    /** The CAN transmit pin */
    Pin txPin;
    /** The CAN receive pin */
    Pin rxPin;
    /** Represents if filtering should take place for CAN ids */
    bool filtering;
    /** If CAN should operate in loop back mode */
    bool loopbackEnabled;

protected:
    /** Function pointer to call for the interrupt handler */
    void (*handler)(CANMessage&, void* priv);
    /** Private data to pass into the IRQ handler */
    void* priv;
};
}// namespace EVT::core::IO

#endif
