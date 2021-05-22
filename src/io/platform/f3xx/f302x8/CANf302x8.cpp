// TODO: Implement choosing between polling and interrupt mode

#include <cstring>
#include <stdint.h>

#include <HALf3/stm32f3xx.h>

#include <EVT/io/platform/f3xx/f302x8/CANf302x8.hpp>
#include <EVT/utils/time.hpp>

namespace EVT::core::IO {

CANf302x8::CANf302x8(Pin txPin, Pin rxPin, uint8_t* CANids, uint8_t numCANids)
    : CAN(txPin, rxPin, CANids, numCANids) {

    // Setup GPIO
    GPIO_InitTypeDef gpioInit = {0};
    Pin canPins[] = {txPin, rxPin};
    gpioInit.Pin = static_cast<uint32_t>(1 <<
        (static_cast<uint32_t>(canPins[0]) & 0x0F));
    gpioInit.Pin |= static_cast<uint32_t>(1 <<
        (static_cast<uint32_t>(canPins[1]) & 0x0F));
    gpioInit.Mode = GPIO_MODE_AF_OD;
    gpioInit.Pull = GPIO_PULLUP;
    gpioInit.Speed = GPIO_SPEED_FREQ_HIGH;
    gpioInit.Alternate = GPIO_AF9_CAN;

    for (uint8_t i = 0; i < 2; i++) {
        switch ((static_cast<uint8_t>(canPins[i]) & 0xF0) >> 4) {
            case 0x0:
                __HAL_RCC_GPIOA_CLK_ENABLE();
                HAL_GPIO_Init(GPIOA, &gpioInit);
                break;
            case 0x1:
                __HAL_RCC_GPIOB_CLK_ENABLE();
                HAL_GPIO_Init(GPIOB, &gpioInit);
                break;
            case 0x2:
                __HAL_RCC_GPIOC_CLK_ENABLE();
                HAL_GPIO_Init(GPIOC, &gpioInit);
                break;
            case 0x3:
                __HAL_RCC_GPIOD_CLK_ENABLE();
                HAL_GPIO_Init(GPIOC, &gpioInit);
                break;
            default:
                break;  // Should never get here
        }
    }

    // Initialize HAL CAN
    halCAN.Instance = CAN1;
    halCAN.Init.Prescaler = 5;
    halCAN.Init.Mode = CAN_MODE_LOOPBACK;
    halCAN.Init.SyncJumpWidth = CAN_SJW_1TQ;
    halCAN.Init.TimeSeg1 = CAN_BS1_1TQ;
    halCAN.Init.TimeSeg2 = CAN_BS2_1TQ;
    halCAN.Init.TimeTriggeredMode = DISABLE;
    halCAN.Init.AutoBusOff = DISABLE;
    halCAN.Init.AutoWakeUp = DISABLE;
    halCAN.Init.AutoRetransmission = DISABLE;
    halCAN.Init.ReceiveFifoLocked = DISABLE;
    halCAN.Init.TransmitFifoPriority = DISABLE;

    __HAL_RCC_CAN1_CLK_ENABLE();
    HAL_CAN_Init(&halCAN);
}

void CANf302x8::transmit(CANMessage& message) {
    CAN_TxHeaderTypeDef txHeader = {0};
    uint8_t payload[CANMessage::CAN_MAX_PAYLOAD_SIZE] = {0};

    uint32_t mailbox = CAN_TX_MAILBOX0;

    txHeader.ExtId = message.getId();
    txHeader.IDE = CAN_ID_EXT;
    // TODO: Consider having remote setting be part of CAN message
    txHeader.RTR = CAN_RTR_DATA;
    txHeader.DLC = message.getDataLength();
    txHeader.TransmitGlobalTime = DISABLE;

    // Copy over bytes
    std::memcpy(payload, message.getPayload(), message.getDataLength());

    // Wait unil mailbox 0 is available TODO: Add interrupt implementation
    while (HAL_CAN_GetTxMailboxesFreeLevel(&halCAN) == 0) {
        time::wait(1);
    }

    HAL_CAN_AddTxMessage(&halCAN, &txHeader, payload, &mailbox);
}

// TODO: Use both RxFifo0 and RxFif1
CANMessage* CANf302x8::receive(CANMessage* message, bool blocking) {
    CAN_RxHeaderTypeDef rxHeader = {0};
    uint8_t payload[CANMessage::CAN_MAX_PAYLOAD_SIZE] = {0};

    bool hasMessage = false;

    // Check to make sure a message is available, if blocking, wait until
    // a message is available.
    do {
        time::wait(1);
        hasMessage = HAL_CAN_GetRxFifoFillLevel(&halCAN, CAN_RX_FIFO0) > 0;
        // If the user does not want to wait for a message, return nullptr
        if (!blocking && !hasMessage)
            return nullptr;
    } while (!hasMessage);

    // Read in message
    HAL_CAN_GetRxMessage(&halCAN, CAN_RX_FIFO0, &rxHeader, payload);

    // Construct CANMessage
    message->setId(rxHeader.ExtId);
    message->setDataLength(rxHeader.DLC);
    message->setPayload(payload);

    return message;
}

}  // namespace EVT::core::IO
