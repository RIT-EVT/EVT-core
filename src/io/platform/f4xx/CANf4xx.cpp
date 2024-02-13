// TODO:
//      * Implement choosing between polling and interrupt mode
//      * Add flexible baud rates
//      * Add hardware based filtering settings
//      * "Thread" safty for queue access
//      * Support hardware based filtering

#include <cstring>

#include <HALf4/stm32f4xx.h>

#include <EVT/io/platform/f4xx/CANf4xx.hpp>
#include <EVT/io/platform/f4xx/GPIOf4xx.hpp>
#include <EVT/platform/f4xx/stm32f4xx.hpp>
#include <EVT/utils/time.hpp>
#include <EVT/utils/types/FixedQueue.hpp>
#include <EVT/utils/log.hpp>

namespace log = EVT::core::log;

#define CAN_INTERFACE_1_INDEX 0
#define CAN_INTERFACE_2_INDEX 1

static EVT::core::IO::CANf4xx* CAN_INTERFACES[2] = {nullptr};

/**
 * Interrupt handler for incoming CAN messages. The messages are added to
 * a queue for receiving.
 *
 * NOTE: Ideally a pointer to the queue would be stored in the CAN handle
 * typedef. It may be weally a pointer to the queue would be stored in the CAN handle
 * typedef. It may be worth considering changing the HAL code.orth considering changing the HAL code.
 *
 * NOTE 2: This is really the only way to do this. If we extend the HAL code it is a massive project.
 * Having a static is not ideal, but it is the best option.
 */
extern "C" void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef* hcan) {
    CAN_RxHeaderTypeDef rxHeader = {0};
    uint8_t payload[EVT::core::IO::CANMessage::CAN_MAX_PAYLOAD_SIZE];


    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rxHeader, payload);

    // Construct the CANMessage
    bool isExtended = rxHeader.IDE == CAN_ID_EXT;
    uint32_t id = isExtended ? rxHeader.ExtId : rxHeader.StdId;
    EVT::core::IO::CANMessage message(id, rxHeader.DLC, payload, isExtended);

    // Retrieve the correct instance of CAN so we access the correct interface in the static interfaces array.
    uint8_t index = (hcan->Instance == CAN1) ? CAN_INTERFACE_1_INDEX : CAN_INTERFACE_2_INDEX;

    // Check to see if a user defined IRQ has been provided
    if (CAN_INTERFACES[index]->triggerIRQ(message))
        return;

    // TODO: Part of an error system should log when the CAN buffer has filled
    CAN_INTERFACES[index]->addCANMessage(message);
}

namespace EVT::core::IO {

static uint8_t getPortID(Pin txPin) {
    switch (txPin) {
    case Pin::PB_8:
        return 1;
    case Pin::PB_6:
        return 2;
    default:
        log::LOGGER.log(log::Logger::LogLevel::ERROR, "Invalid CAN Pin");
        return 0;
    }
}

static void getInstance(CAN_HandleTypeDef *halCAN, uint8_t portID) {
    switch (portID) {
    case 1:
        halCAN->Instance = CAN1;
        __HAL_RCC_CAN1_CLK_ENABLE();
        HAL_CAN_Init(halCAN);

        // Intialize interrupts
        HAL_CAN_ActivateNotification(halCAN, CAN_IT_RX_FIFO0_MSG_PENDING);
        HAL_NVIC_SetPriority(IRQn_Type::CAN1_RX0_IRQn, EVT::core::platform::CAN_INTERRUPT_PRIORITY, 0);
        HAL_NVIC_EnableIRQ(IRQn_Type::CAN1_RX0_IRQn);
        break;
    case 2:
        halCAN->Instance = CAN2;
        __HAL_RCC_CAN2_CLK_ENABLE();
        HAL_CAN_Init(halCAN);

        // Intialize interrupts
        HAL_CAN_ActivateNotification(halCAN, CAN_IT_RX_FIFO0_MSG_PENDING);
        HAL_NVIC_SetPriority(IRQn_Type::CAN2_RX0_IRQn, EVT::core::platform::CAN_INTERRUPT_PRIORITY, 0);
        HAL_NVIC_EnableIRQ(IRQn_Type::CAN2_RX0_IRQn);
        break;
    default:
        break;
    }
}


CANf4xx::CANf4xx(Pin txPin, Pin rxPin, bool loopbackEnabled): CAN(txPin, rxPin, loopbackEnabled) {
    // Setup GPIO
    portID = getPortID(txPin);

    GPIO_InitTypeDef gpioInit = {0};
    Pin canPins[] = {txPin, rxPin};
    uint8_t numOfPins = 2;
    uint8_t gpioAlt = GPIO_AF9_CAN1; // This is okay I promise. GPIO_AF9_CAN1 and GPIO_AF9_CAN2 are BOTH 0x09.

    GPIOf4xx::gpioStateInit(&gpioInit, canPins, numOfPins, GPIO_MODE_AF_OD,GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH, gpioAlt);
}

CAN::CANStatus CANf4xx::connect(bool autoBusOff) {
    // Initialize HAL CAN
    // Bit timing values calculated from the website
    // http://www.bittiming.can-wiki.info/
    uint32_t mode = loopbackEnabled ? CAN_MODE_LOOPBACK : CAN_MODE_NORMAL;
    halCAN.Init.Prescaler = 1;
    halCAN.Init.Mode = mode;
    halCAN.Init.SyncJumpWidth = CAN_SJW_1TQ;
    halCAN.Init.TimeSeg1 = CAN_BS1_13TQ;
    halCAN.Init.TimeSeg2 = CAN_BS2_2TQ;
    halCAN.Init.TimeTriggeredMode = DISABLE;
    if (autoBusOff) {
        halCAN.Init.AutoBusOff = ENABLE;
    } else {
        halCAN.Init.AutoBusOff = DISABLE;
    }
    halCAN.Init.AutoWakeUp = DISABLE;
    halCAN.Init.AutoRetransmission = DISABLE;
    halCAN.Init.ReceiveFifoLocked = DISABLE;
    halCAN.Init.TransmitFifoPriority = DISABLE;

    // Add into the static array of interfaces.
    switch (portID) {
    case 1:
        CAN_INTERFACES[CAN_INTERFACE_1_INDEX] = this;
        break;
    case 2:
        CAN_INTERFACES[CAN_INTERFACE_2_INDEX] = this;
        break;
    default:
        break;
    }

    getInstance(&halCAN, portID);

    // By default - filter that accepts all incoming messages
    CAN_FilterTypeDef defaultFilter;
    defaultFilter.FilterIdHigh = 0;
    defaultFilter.FilterIdLow = 0;
    defaultFilter.FilterMaskIdHigh = 0x0000;
    defaultFilter.FilterMaskIdLow = 0xFFFF;
    defaultFilter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    defaultFilter.FilterBank = 0;
    defaultFilter.FilterMode = CAN_FILTERMODE_IDMASK;
    defaultFilter.FilterScale = CAN_FILTERSCALE_16BIT;
    defaultFilter.FilterActivation = ENABLE;

    if (HAL_CAN_ConfigFilter(&halCAN, &defaultFilter) != HAL_OK) {
        return CANStatus::ERROR;
    }

    if (HAL_CAN_Start(&halCAN) != HAL_OK) {
        return CANStatus::ERROR;
    }

    return CANStatus::OK;
}

CAN::CANStatus CANf4xx::disconnect() {
    if (HAL_CAN_Stop(&halCAN) != HAL_OK) {
        return CANStatus::ERROR;
    }
    return CANStatus::OK;
}

CAN::CANStatus CANf4xx::transmit(CANMessage& message) {
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

    // Wait until there is a free mailbox available for use. Timeout is necessary when there is no
    // CAN network available. If there is no CAN network the mailboxes will fill and this function
    // will infinitely loop.
    uint8_t timeout = 0;
    while (HAL_CAN_GetTxMailboxesFreeLevel(&halCAN) == 0 && timeout < EVT_CAN_TIMEOUT) {
        time::wait(1);
        timeout++;
    }

    // If the mailbox is still full, return a timeout error
    if (HAL_CAN_GetTxMailboxesFreeLevel(&halCAN) == 0) {
        return CANStatus::TIMEOUT;
    }

    HAL_StatusTypeDef result = HAL_CAN_AddTxMessage(&halCAN, &txHeader, payload, &mailbox);

    switch (result) {
    case HAL_OK:
        return CAN::CANStatus::OK;
    case HAL_TIMEOUT:
        return CAN::CANStatus::TIMEOUT;
    default:
        return CAN::CANStatus::ERROR;
    }
}

// TODO: Use both RxFifo0 and RxFif1
CAN::CANStatus CANf4xx::receive(CANMessage* message, bool blocking) {
    bool hasMessage = false;

    // Check to make sure a message is available, if blocking, wait until
    // a message is available.
    do {
        hasMessage = !messageQueue.isEmpty();
        // If the user does not want to wait for a message, return nullptr
        if (!blocking && !hasMessage)
            return CANStatus::TIMEOUT;
        time::wait(1);
    } while (!hasMessage);

    // Return the message from the queue;
    if (messageQueue.pop(message)) {
        return CANStatus::OK;
    } else {
        return CANStatus::TIMEOUT;
    }
}

CAN::CANStatus CANf4xx::addCANFilter(uint16_t filterExplicitId, uint16_t filterMask, uint8_t filterBank) {
    CAN_FilterTypeDef newFilter;
    newFilter.FilterIdHigh = filterExplicitId << 5;//must shift 11-bits to MSB of 16-bits
    newFilter.FilterIdLow = 0;
    newFilter.FilterMaskIdHigh = filterMask;
    newFilter.FilterMaskIdLow = 0xFFFF;//block off second filter with all 1s
    newFilter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    newFilter.FilterBank = filterBank;
    newFilter.FilterMode = CAN_FILTERMODE_IDMASK;
    newFilter.FilterScale = CAN_FILTERSCALE_16BIT;
    newFilter.FilterActivation = ENABLE;

    if (HAL_CAN_ConfigFilter(&halCAN, &newFilter) != HAL_OK) {
        return CANStatus::ERROR;
    }
    return CANStatus::OK;
}

CAN::CANStatus CANf4xx::enableEmergencyFilter(uint32_t state) {
    CAN_FilterTypeDef emergencyFilter;

    emergencyFilter.FilterIdHigh = 0x1000;//only 0001 (emergency code) allowed
    emergencyFilter.FilterIdLow = 0;
    emergencyFilter.FilterMaskIdHigh = 0xF000;//1111000000000000 Only looking for 4-bit code
    emergencyFilter.FilterMaskIdLow = 0xFFFF; //block off second filter with all 1s
    emergencyFilter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    emergencyFilter.FilterBank = 1;
    emergencyFilter.FilterMode = CAN_FILTERMODE_IDMASK;
    emergencyFilter.FilterScale = CAN_FILTERSCALE_16BIT;
    emergencyFilter.FilterActivation = state;

    if (HAL_CAN_ConfigFilter(&halCAN, &emergencyFilter) != HAL_OK) {
        return CANStatus::ERROR;
    }
    return CANStatus::OK;
}

void CANf4xx::addCANMessage(CANMessage& message) {
    if (messageQueue.canInsert())
        messageQueue.append(message);
}

bool CANf4xx::triggerIRQ(CANMessage& message) {
    if (handler == nullptr)
        return false;
    handler(message, priv);
    return true;
}

}// namespace EVT::core::IO
