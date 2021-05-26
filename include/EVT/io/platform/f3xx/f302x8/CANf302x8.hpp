#ifndef _EVT_CANf302x8_
#define _EVT_CANf302x8_

#include <stdint.h>

#include <HALf3/stm32f3xx.h>
#include <HALf3/stm32f3xx_hal_can.h>

#include <EVT/io/CAN.hpp>
#include <EVT/utils/types/FixedQueue.hpp>

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
     * @param txPin The pin to trasmit CAN messages on
     * @param rxPin The pin to receive CAN messages on
     * @param CANids Array of CAN ids to recieve, leaving empty allows
     *      all CAN messages to be received
     * @param numCANids The number of CAN ids in the provided array of
     *      CAN ids, zero means allow all CAN ids
     */
    CANf302x8(Pin txPin, Pin rxPin, uint8_t* CANids, uint8_t numCANids);

    /**
     * Send a message over CAN
     *
     * @param message THe mes   sage to send over CAN.
     */
    void transmit(CANMessage& message);

    /**
     * Receive a message over CAN. The user can either receive in blocking or
     * non-blocking mode. In blocking mode, the code will hang until a message
     * is received then return a pointer to the message that was passed in.
     * In non-blocking, a nullptr will be returned if no message is currently
     * in the mailbox.
     *
     * NOTE: If you add your own custom callback. This method will alway
     * return a nullptr. If you leave this as blocking, then the program will
     * always block.
     *
     * @param message {out} The message to populate with data
     * @param blocking Used to determine if received should block or not, by
     *      default receive is blocking
     * @return A pointer to the passed in message, nullptr if message not
     *      received.
     */
    CANMessage* receive(CANMessage* message, bool blocking=true);

private:
    /** Instance of the HAL can interface */
    CAN_HandleTypeDef halCAN;
    /** Queue which holds received CAN messages */
    EVT::core::types::FixedQueue<CANMessage> messageQueue;

};


}  // namespace EVT::core::IO

#endif
