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
        OK = 0,
        TIMEOUT = 1,
        ERROR = 2
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
     * Join the CAN network. Will attempt to connect to the CAN network
     * and return the cooresponding status.
     *
     * @return The status associated with the success of joing the network
     */
    virtual CANStatus connect() = 0;

    /**
     * Disconnect from the CAN network.
     *
     * @return The status associated with attempting to disconnect
     */
    virtual CANStatus disconnect() = 0;

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
     * Instantiates a new CAN filter using the 16-bit ID-Mask mode. If a filter bank
     * that is already in use is provided, the existing filter will be overwritten.
     * A second pair of filter Id and mask can be given to the same filter bank.
     * 
     * @param[in] filterExplicitId 11-bit identifier that must be an exact match to pass
     * @param[in] filterMask 16-bit mask where [1] means care and [0] means don't care
     * @param[in] filterBank value between 0-13 where the filter info is stored
     * @return The status associated with adding a new CAN message filter
     */
    virtual CANStatus addCANFilter(uint16_t filterExplicitId, uint16_t filterMask, uint8_t filterBank);

    /**
     * Enable or disable a filter that lets through any CAN messages that, following the CANopen
     * standard, begin with the Emergency code of 001
     * 
     * @param[in] state Enum passed to set filter functional state
     * @return The status associated with setting the energency filter state
     */
    virtual CANStatus enableEmergencyFilter(uint32_t state);

    /**
     * Instantiates a new CAN filter using the 16-bit ID-Mask mode. If a filter bank
     * that is already in use is provided, the existing filter will be overwritten.
     * A second pair of filter Id and mask can be given to the same filter bank.
     * 
     * @param[in] filterExplicitId 11-bit identifier that must be an exact match to pass
     * @param[in] filterMask 16-bit mask where [1] means care and [0] means don't care
     * @param[in] filterBank value between 0-13 where the filter info is stored
     */
    virtual void addCANFilter(uint16_t filterExplicitId, uint16_t filterMask, uint8_t filterBank);

    /**
     * Enable or disable a filter that lets through any CAN messages that, following the CANopen
     * standard, begin with the Emergency code of 001
     * 
     * @param[in] state Enum passed to set filter functional state
     */
    virtual void enableEmergencyFilter(uint32_t state);

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

protected:
    /** Function pointer to call for the interrupt handler */
    void (*handler)(CANMessage&, void* priv);
    /** Private data to pass into the IRQ handler */
    void* priv;
    /** If CAN should operate in loop back mode */
    bool loopbackEnabled;
};
}// namespace EVT::core::IO

#endif
