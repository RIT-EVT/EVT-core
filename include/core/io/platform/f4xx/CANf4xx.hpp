#ifndef _EVT_CANf4xx_
#define _EVT_CANf4xx_

#include <stdint.h>

#include <HALf4/stm32f4xx.h>
#include <HALf4/stm32f4xx_hal_can.h>

#include <core/io/CAN.hpp>
#include <core/utils/types/FixedQueue.hpp>

// Allows for resizable CAN queue if needed
#ifndef CAN_MESSAGE_QUEUE_SIZE
    #define CAN_MESSAGE_QUEUE_SIZE 100
#endif

namespace core::io {

/**
 * STM32f4xx implementation of the CAN protocol. The STM32f4xx has an
 * onboard CAN controller which adds additional features.
 *
 * 1. Ability to generate interrupts which allows users to add custom call
 * backs
 *
 * 2. Hardware based message filtering which allows the filtering of messages
 * to be handled by hardware not software.
 */
class CANf4xx : public CAN {
public:
    /**
     * Create a new instance of an STM32f4xx CAN interface
     *
     * @param[in] txPin The pin to trasmit CAN messages on
     * @param[in] rxPin The pin to receive CAN messages on
     * @param[in] loopbackEnabled Flag for enabling CAN loop back
     */
    CANf4xx(Pin txPin, Pin rxPin, bool loopbackEnabled = false);

    /**
     * Attempt to join the CAN network.
     *
     * For the STM32f4xx this involves attempting to startup the CAN
     * interface. This could cause an error in the case of invalid
     * parameters.
     *
     * @param[in] autoBusOff Indicates the state halCAN.Init.AutoBusOff should be in
     *
     * @return CANStatus::OK on success, CANStatus::ERROR otherwise
     */
    CANStatus connect(bool autoBusOff = false) override;

    /**
     * Disconnect from the CAN network.
     *
     * For the STM32f4xx this involves disconnect from the CAN interface.
     *
     * @return CANStatus::OK if we successfully de-init the CAN interface
     */
    CANStatus disconnect() override;

    CANStatus transmit(CANMessage& message) override;

    CANStatus receive(CANMessage* message, bool blocking = false) override;

    CANStatus addCANFilter(uint16_t filterExplicitId, uint16_t filterMask, uint8_t filterBank) override;

    CANStatus enableEmergencyFilter(uint32_t state) override;

    void addIRQHandler(void (*handler)(CANMessage&, void* priv), void* priv);

    /**
     * Add a message to the CAN receive queue.
     *
     * NOTE: This is public for use with the STM HAL interrupt handler. This
     * method should not be used outside of that application.
     *
     * @param[in] message The CANmessage to add to the receive queue
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
     * @param[in] message The message to pass to the interrupt handler
     * @return True if the interrupt handler exists and has handled the message
     */
    bool triggerIRQ(CANMessage& message);

    /** The filter bank index for emergency CAN messages */
    static constexpr uint8_t EMERGENCY_FILTER_BANK_INDEX = 1;

    /** Second filter bank start index*/
    static constexpr uint8_t SECOND_FILTER_BANK_INDEX = 14;

private:
    /**
     * Get the CAN ID that is associated with the given pin
     * combination. This information is pulled from the STM32F446xx
     * documentation with easier documentation on in CUBEMX
     *
     * @param sclPin The selected I2C clock pin
     * @return The port ID associated with the selected pins
     */
    static uint8_t getPortID(Pin txPin, Pin rxPin);

    /** Instance of the HAL can interface */
    CAN_HandleTypeDef halCAN;
    /** Queue which holds received CAN messages */
    core::types::FixedQueue<CAN_MESSAGE_QUEUE_SIZE, CANMessage> messageQueue;
};

} // namespace core::io

#endif
