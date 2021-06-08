// TODO: Implement choosing between polling and interrupt mode

#include <cstring>
#include <stdint.h>

#include <HALf3/stm32f3xx.h>

#include <EVT/io/platform/f3xx/f302x8/CANf302x8.hpp>
#include <EVT/utils/time.hpp>
#include <EVT/utils/types/FixedQueue.hpp>


namespace {

// Pointer to a queue element that will store the CAN messages. This is made
// global so that the CAN interrupts can have access to the queue. In a more
// ideal situation, the passed in HAL CAN instance would contain the queue
// as "private data". This should be considered for the future.
//
// NOTE: Part of the reason this works is because the STM32F3xx only supports
// a single CAN interface at a time.
EVT::core::types::FixedQueue<EVT::core::IO::CANMessage>* canMessageQueue;
// Pointer to the CAN interface, made global for similar reasons to the
// variable above.
CAN_HandleTypeDef* hcan;

}

extern "C" void CAN_RX_IRQHandler(void) {
    HAL_CAN_IRQHandler(hcan);
}

/**
 * Interrupt handler for incoming CAN messages. The messages are added to
 * a queue for receiving.
 *
 * NOTE: Ideally a pointer to the queue would be stored in the CAN handle
 * typedef. It may be worth considering changing the HAL code.
 */
extern "C" void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef* hcan) {
    CAN_RxHeaderTypeDef rxHeader = {0};
    uint8_t payload[EVT::core::IO::CANMessage::CAN_MAX_PAYLOAD_SIZE];

    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rxHeader, payload);

    EVT::core::IO::CANMessage message(rxHeader.ExtId, rxHeader.DLC, payload);
    // TODO: Part of an error system should log when the CAN buffer has filled
    if (canMessageQueue->canInsert())
        canMessageQueue->append(message);
}

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
    // Bit timing values calculated from the website
    // http://www.bittiming.can-wiki.info/
    halCAN.Instance                     = CAN1;
    halCAN.Init.Prescaler               = 1;
    halCAN.Init.Mode                    = CAN_MODE_LOOPBACK;
    halCAN.Init.SyncJumpWidth           = CAN_SJW_1TQ;
    halCAN.Init.TimeSeg1                = CAN_BS1_13TQ;
    halCAN.Init.TimeSeg2                = CAN_BS2_2TQ;
    halCAN.Init.TimeTriggeredMode       = DISABLE;
    halCAN.Init.AutoBusOff              = DISABLE;
    halCAN.Init.AutoWakeUp              = DISABLE;
    halCAN.Init.AutoRetransmission      = DISABLE;
    halCAN.Init.ReceiveFifoLocked       = DISABLE;
    halCAN.Init.TransmitFifoPriority    = DISABLE;

    // Setup global variables
    hcan = &this->halCAN;
    canMessageQueue = &this->messageQueue;

    // Intialize interrupts
    HAL_CAN_ActivateNotification(&halCAN, CAN_IT_RX_FIFO0_MSG_PENDING);
    HAL_NVIC_SetPriority(CAN_RX0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(CAN_RX0_IRQn);
    // NVIC_SetVector(CAN_RX0_IRQn, (uint32_t)&CAN_RX_IRQHandler);

    /* By default - filter that accepts all incoming messages */
    CAN_FilterTypeDef defaultFilter;
    defaultFilter.FilterIdHigh         = 0;
    defaultFilter.FilterIdLow          = 0;
    defaultFilter.FilterMaskIdHigh     = 0;
    defaultFilter.FilterMaskIdLow      = 0;
    defaultFilter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    defaultFilter.FilterBank           = 0;
    defaultFilter.FilterMode           = CAN_FILTERMODE_IDMASK;
    defaultFilter.FilterScale          = CAN_FILTERSCALE_32BIT;
    defaultFilter.FilterActivation     = ENABLE;

    HAL_CAN_ConfigFilter(&halCAN, &defaultFilter);

    __HAL_RCC_CAN1_CLK_ENABLE();
    HAL_CAN_Init(&halCAN);
    HAL_CAN_ActivateNotification(&halCAN, CAN_IT_RX_FIFO0_MSG_PENDING);
    HAL_CAN_Start(&halCAN);
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
    bool hasMessage = false;

    // Check to make sure a message is available, if blocking, wait until
    // a message is available.
    do {
        hasMessage = !messageQueue.isEmpty();
        // If the user does not want to wait for a message, return nullptr
        if (!blocking && !hasMessage)
            return nullptr;
        time::wait(1);
    } while (!hasMessage);

    // Return the message from the queue;
    if (messageQueue.pop(message)) {
        return message;
    } else {
        return nullptr;
    }
}

}  // namespace EVT::core::IO
