// TODO:
//      * Implement choosing between polling and interrupt mode
//      * Add flexible baud rates
//      * Add hardware based filtering settings
//      * "Thread" safty for queue access
//      * Support hardware based filtering

#include <cstring>
#include <stdint.h>

#include <HALf3/stm32f3xx.h>

#include <EVT/io/platform/f3xx/f302x8/CANf302x8.hpp>
#include <EVT/io/platform/f3xx/f302x8/GPIOf302x8.hpp>
#include <EVT/platform/f3xx/stm32f302x8.hpp>
#include <EVT/utils/time.hpp>
#include <EVT/utils/types/FixedQueue.hpp>

namespace {

// Pointer to the CANf302r8 interface
//
// NOTE: Part of the reason this works is because the STM32F3xx only supports
// a single CAN interface at a time.
EVT::core::IO::CANf302x8* canIntf;

// Pointer to the CAN interface, made global for similar reasons to the
// variable above.
CAN_HandleTypeDef* hcan;

}// namespace

extern "C" void CAN_RX0_IRQHandler(void) {
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

    // Construct the CANmessage
    bool isExtended = rxHeader.IDE == CAN_ID_EXT;
    uint32_t id = isExtended ? rxHeader.ExtId : rxHeader.StdId;
    EVT::core::IO::CANMessage message(id, rxHeader.DLC, payload, isExtended);

    // Check to see if a user defined IRQ has been provided
    if (canIntf->triggerIRQ(message))
        return;

    // TODO: Part of an error system should log when the CAN buffer has filled
    canIntf->addCANMessage(message);
}

namespace EVT::core::IO {

CANf302x8::CANf302x8(Pin txPin, Pin rxPin, bool loopbackEnabled)
    : CAN(txPin, rxPin, loopbackEnabled) {

    // Setup GPIO
    GPIO_InitTypeDef gpioInit = {0};
    Pin canPins[] = {txPin, rxPin};
    uint8_t numOfPins = 2;
    GPIOf302x8::gpioStateInit(&gpioInit, canPins, numOfPins, GPIO_MODE_AF_OD,
                              GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH, GPIO_AF9_CAN);

    // Initialize HAL CAN
    // Bit timing values calculated from the website
    // http://www.bittiming.can-wiki.info/
    uint32_t mode = loopbackEnabled ? CAN_MODE_LOOPBACK : CAN_MODE_NORMAL;
    halCAN.Instance = CAN1;
    halCAN.Init.Prescaler = 1;
    halCAN.Init.Mode = mode;
    halCAN.Init.SyncJumpWidth = CAN_SJW_1TQ;
    halCAN.Init.TimeSeg1 = CAN_BS1_13TQ;
    halCAN.Init.TimeSeg2 = CAN_BS2_2TQ;
    halCAN.Init.TimeTriggeredMode = DISABLE;
    halCAN.Init.AutoBusOff = DISABLE;
    halCAN.Init.AutoWakeUp = DISABLE;
    halCAN.Init.AutoRetransmission = DISABLE;
    halCAN.Init.ReceiveFifoLocked = DISABLE;
    halCAN.Init.TransmitFifoPriority = DISABLE;

    // Setup global variables
    hcan = &this->halCAN;
    canIntf = this;

    __HAL_RCC_CAN1_CLK_ENABLE();
    HAL_CAN_Init(&halCAN);

    // Intialize interrupts
    HAL_CAN_ActivateNotification(&halCAN, CAN_IT_RX_FIFO0_MSG_PENDING);
    HAL_NVIC_SetPriority(CAN_RX0_IRQn, EVT::core::platform::CAN_INTERRUPT_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(CAN_RX0_IRQn);

    /* By default - filter that accepts all incoming messages */
    CAN_FilterTypeDef defaultFilter;
    defaultFilter.FilterIdHigh = 0;
    defaultFilter.FilterIdLow = 0;
    defaultFilter.FilterMaskIdHigh = 0;
    defaultFilter.FilterMaskIdLow = 0;
    defaultFilter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    defaultFilter.FilterBank = 0;
    defaultFilter.FilterMode = CAN_FILTERMODE_IDMASK;
    defaultFilter.FilterScale = CAN_FILTERSCALE_32BIT;
    defaultFilter.FilterActivation = ENABLE;

    HAL_CAN_ConfigFilter(&halCAN, &defaultFilter);

    // /* Test filter for CANopen emergency code */
    // CAN_FilterTypeDef emergencyFilter;
    // emergencyFilter.FilterIdHigh = 0x1000;
    // emergencyFilter.FilterIdLow = 0x0000;
    // emergencyFilter.FilterMaskIdHigh = 0xF000;
    // emergencyFilter.FilterMaskIdLow = 0x0000;
    // emergencyFilter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    // emergencyFilter.FilterBank = 1;
    // emergencyFilter.FilterMode = CAN_FILTERMODE_IDMASK;
    // emergencyFilter.FilterScale = CAN_FILTERSCALE_16BIT;
    // emergencyFilter.FilterActivation = ENABLE;

    // HAL_CAN_ConfigFilter(&halCan, &emergencyFilter);

    // /* Test filter for 7-bit device id */
    // CAN_FilterTypeDef deviceFilter;
    // deviceFilter.FilterIdHigh = nodeId;
    // deviceFilter.FilterIdLow = 0x0000;
    // deviceFilter.FilterMaskIdHigh = 0x0FE0;
    // deviceFilter.FilterMaskIdLow = 0x0000;
    // deviceFilter.FilterFIFOAssignment = CAN_FILTER_FIFO1;
    // deviceFilter.FilterBank = 2;
    // deviceFilter.FilterMode = CAN_FILTERMODE_IDMASK;
    // deviceFilter.FilterScale = CAN_FILTERSCALE_16BIT;
    // deviceFilter.FilterActivation = ENABLE;

    // HAL_CAN_ConfigFilter(&halCan, &deviceFilter)

    HAL_CAN_Start(&halCAN);
}

void CANf302x8::transmit(CANMessage& message) {
    CAN_TxHeaderTypeDef txHeader = {0};
    uint8_t payload[CANMessage::CAN_MAX_PAYLOAD_SIZE] = {0};

    uint32_t mailbox = CAN_TX_MAILBOX0;

    // Set the message ID
    if (message.isCANExtended())
        txHeader.ExtId = message.getId();
    else
        txHeader.StdId = message.getId();
    txHeader.IDE = message.isCANExtended() ? CAN_ID_EXT : CAN_ID_STD;
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

void CANf302x8::addCANMessage(CANMessage& message) {
    if (messageQueue.canInsert())
        messageQueue.append(message);
}

bool CANf302x8::triggerIRQ(CANMessage& message) {
    if (handler == nullptr)
        return false;
    handler(message, priv);
    return true;
}

}// namespace EVT::core::IO
