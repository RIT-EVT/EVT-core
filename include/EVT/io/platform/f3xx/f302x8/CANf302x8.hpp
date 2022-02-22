#ifndef _EVT_CANf302x8_
#define _EVT_CANf302x8_

#include <stdint.h>

#include <HALf3/stm32f3xx.h>
#include <HALf3/stm32f3xx_hal_can.h>

#include <EVT/io/CAN.hpp>
#include <EVT/utils/types/FixedQueue.hpp>

#define CAN_MESSAGE_QUEUE_SIZE 100

namespace EVT::core::IO {

/**
 * STMF302x8 implementation of the CAN protocol. The STM32f302x8 has an on
 * board CAN controller which adds additional features.
 *
 * 1. Ability to generate interrupts which allows users to add custom call
 * backs
 *
 * 2. Hardware based message filtering which allows the filtering of messages
 * to be handled by hardware not software.
 */
class CANf302x8 : public CAN {
public:
    /**
     * Create a new instance of an STM32f302x8 CAN interface
     *
     * @param txPin[in] The pin to trasmit CAN messages on
     * @param rxPin[in] The pin to receive CAN messages on
     * @param loopbackEnabled[in] Flag for enabling CAN loop back
     */
    CANf302x8(Pin txPin, Pin rxPin, bool loopbackEnabled = false);

    /**
     * @copydoc EVT::core::IO::CAN::transmit
     */
    void transmit(CANMessage& message);

    /**
     * @copydoc EVT::core::IO::CAN::receive
     */
    CANMessage* receive(CANMessage* message, bool blocking = false);

    /**
     * @copydoc EVT::core::IO::CAN::addCANFilter
     */
    void addCANFilter(uint16_t filterExplicitId, uint16_t filterMask, uint8_t filterBank);

    /**
     * @copydoc EVT::core::IO::CAN::enableEmergencyFilter
     */
    void enableEmergencyFilter(uint32_t state);

    /**
     * @copydoc EVT::core::IO::CAN::addIRQHandler
     */
    void addIRQHandler(void (*handler)(CANMessage&, void* priv), void* priv);

    /**
     * Add a message to the CAN receive queue.
     *
     * NOTE: This is public for use with the STM HAL interrupt handler. This
     * method should not be used outside of that application.
     *
     * @param message[in] The CANmessage to add to the receive queue
     */
    void addCANMessage(CANMessage& message);

    /**
     * Manually trigger the user specified interrupt handler. This is intended
     * to be used by the STM HAL interrupt handler and generally should
     * not be used beyound that use case.
     *
     * NOTE: This is public for use with the STM HAL interrupt handler. This
     * method should not be used outside of that application.
     *
     * @param message[in] The message to pass to the interrupt handler
     * @return True if the interrupt handler exists and has handled the message
     */
    bool triggerIRQ(CANMessage& message);

private:
    /** Instance of the HAL can interface */
    CAN_HandleTypeDef halCAN;
    /** Queue which holds received CAN messages */
    EVT::core::types::FixedQueue<CAN_MESSAGE_QUEUE_SIZE, CANMessage> messageQueue;
};

}// namespace EVT::core::IO

#endif
