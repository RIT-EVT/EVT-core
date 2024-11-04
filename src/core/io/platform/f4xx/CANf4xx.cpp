

#include <cstring>
#include <stdint.h>

#include <HALf4/stm32f4xx.h>

#include <core/io/pin.hpp>
#include <core/io/platform/f4xx/CANf4xx.hpp>
#include <core/io/platform/f4xx/GPIOf4xx.hpp>
#include <core/platform/f4xx/stm32f4xx.hpp>
#include <core/utils/log.hpp>
#include <core/utils/time.hpp>
#include <core/utils/types/FixedQueue.hpp>

namespace {

/**
 * Struct to hold the global pointers to the CAN F4 interfaces. This is necessary
 * for the interrupts to have a call back to the classes.
 */
struct CANf4Global {
    // Pointer to the CANf446xx interface
    core::io::CANf4xx* canInstance = nullptr;

    // Pointer to the CAN handler for HAL,
    CAN_HandleTypeDef* canHandler = nullptr;
};

struct CANf4Global globalCAN[2];

// Interrupt handler for CAN 1 or index 0 in the global CAN array
extern "C" void CAN1_RX0_IRQHandler(void) {
    if (globalCAN[0].canHandler != nullptr) {
        HAL_CAN_IRQHandler(globalCAN[0].canHandler);
    }
}

// Interrupt handler for CAN 2 or index 1 in the global CAN array
extern "C" void CAN2_RX0_IRQHandler(void) {
    if (globalCAN[1].canHandler != nullptr) {
        HAL_CAN_IRQHandler(globalCAN[1].canHandler);
    }
}

/**
 * HAL interrupt handler for incoming CAN messages. The messages are added to
 * a queue for receiving.
 */
extern "C" void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef* hcan) {
    CAN_RxHeaderTypeDef rxHeader = {0};
    uint8_t payload[core::io::CANMessage::CAN_MAX_PAYLOAD_SIZE];

    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rxHeader, payload);

    // Construct the CANmessage
    bool isExtended = rxHeader.IDE == CAN_ID_EXT;
    uint32_t id     = isExtended ? rxHeader.ExtId : rxHeader.StdId;
    core::io::CANMessage message(id, rxHeader.DLC, payload, isExtended);

    core::io::CANf4xx* canInstance;
    if (hcan == globalCAN[0].canHandler) {
        canInstance = globalCAN[0].canInstance;
    } else {
        canInstance = globalCAN[1].canInstance;
    }

    // Check to see if a user defined IRQ has been provided
    if (canInstance->triggerIRQ(message))
        return;

    canInstance->addCANMessage(message);
}

} // namespace

namespace core::io {

/**
 * Get the CAN ID that is associated with the given pin
 * combination. This information is pulled from the STM32F446xx
 * documentation with easier documentation on in CUBEMX
 *
 * @param sclPin The selected I2C clock pin
 * @return The port ID associated with the selected pins
 */
static uint8_t getPortID(Pin txPin, Pin rxPin) {
    uint8_t txPort = 0;
    uint8_t rxPort = 0;

#ifdef STM32F446xx
    switch (txPin) {
    case Pin::PA_12:
    case Pin::PB_9:
        txPort = 1;
        break;
    case Pin::PB_6:
    case Pin::PB_13:
        txPort = 2;
        break;
    default:
        log::LOGGER.log(log::Logger::LogLevel::ERROR, "Invalid CAN TX Pin");
        txPort = 0;
        break;
    }

    switch (rxPin) {
    case Pin::PA_11:
    case Pin::PB_8:
        rxPort = 1;
        break;
    case Pin::PB_5:
    case Pin::PB_12:
        rxPort = 2;
        break;
    default:
        log::LOGGER.log(log::Logger::LogLevel::ERROR, "Invalid CAN RX Pin");
        rxPort = 0;
        break;
    }
#endif

    if (txPort == rxPort) {
        return txPort;
    }

    return 0;
}

CANf4xx::CANf4xx(Pin txPin, Pin rxPin, bool loopbackEnabled) : CAN(txPin, rxPin, loopbackEnabled) {

    // Setup GPIO
    GPIO_InitTypeDef gpioInit = {0};
    Pin canPins[]             = {txPin, rxPin};
    uint8_t numOfPins         = 2;

    // Both CAN 1 and CAN 2 use AF9 for the GPIO so initialization can be identical
    GPIOf4xx::gpioStateInit(
        &gpioInit, canPins, numOfPins, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF9_CAN1);
}

CAN::CANStatus CANf4xx::connect(bool autoBusOff) {
    // Initialize HAL CAN
    // Bit timing values calculated from the website
    // http://www.bittiming.can-wiki.info/
    uint32_t mode  = loopbackEnabled ? CAN_MODE_LOOPBACK : CAN_MODE_NORMAL;
    uint8_t portID = getPortID(txPin, rxPin);

    IRQn_Type IRQn;

    // Select the instance to use from the pins.
    switch (portID) {
    case 1:
        halCAN.Instance = CAN1;
        IRQn            = CAN1_RX0_IRQn;
        __HAL_RCC_CAN1_CLK_ENABLE();
        break;
    case 2:
        halCAN.Instance = CAN2;
        IRQn            = CAN2_RX0_IRQn;
        __HAL_RCC_CAN1_CLK_ENABLE();
        __HAL_RCC_CAN2_CLK_ENABLE();
        break;
    default:
        log::LOGGER.log(log::Logger::LogLevel::ERROR, "Failed to initalized CAN, Bad pin configuration");
        return CANStatus::ERROR;
    }

    // Configure the rest of the parameters.
    halCAN.Init.Prescaler         = (HAL_RCC_GetHCLKFreq() / DEFAULT_BAUD / 20); // At 50MHz prescaler of 5
    halCAN.Init.Mode              = mode;
    halCAN.Init.SyncJumpWidth     = CAN_SJW_1TQ;
    halCAN.Init.TimeSeg1          = CAN_BS1_8TQ;
    halCAN.Init.TimeSeg2          = CAN_BS2_1TQ;
    halCAN.Init.TimeTriggeredMode = DISABLE;
    if (autoBusOff) {
        halCAN.Init.AutoBusOff = ENABLE;
    } else {
        halCAN.Init.AutoBusOff = DISABLE;
    }
    halCAN.Init.AutoWakeUp           = DISABLE;
    halCAN.Init.AutoRetransmission   = DISABLE;
    halCAN.Init.ReceiveFifoLocked    = DISABLE;
    halCAN.Init.TransmitFifoPriority = DISABLE;

    // Setup global variables
    globalCAN[portID - 1].canHandler  = &this->halCAN;
    globalCAN[portID - 1].canInstance = this;

    HAL_CAN_Init(&halCAN);

    // Intialize interrupts
    HAL_CAN_ActivateNotification(&halCAN, CAN_IT_RX_FIFO0_MSG_PENDING);

    HAL_NVIC_SetPriority(IRQn, core::platform::CAN_INTERRUPT_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(IRQn);

    if (addCANFilter(0, 0, 0) != CANStatus::OK) {
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
    CAN_TxHeaderTypeDef txHeader                      = {0};
    uint8_t payload[CANMessage::CAN_MAX_PAYLOAD_SIZE] = {0};

    uint32_t mailbox = CAN_TX_MAILBOX0;

    // Set the message ID
    if (message.isCANExtended())
        txHeader.ExtId = message.getId();
    else
        txHeader.StdId = message.getId();
    txHeader.IDE                = message.isCANExtended() ? CAN_ID_EXT : CAN_ID_STD;
    txHeader.RTR                = CAN_RTR_DATA;
    txHeader.DLC                = message.getDataLength();
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
    if (halCAN.Instance == CAN1) {
        newFilter.FilterBank = filterBank;
    } else {
        newFilter.FilterBank = filterBank + CANf4xx::SECOND_FILTER_BANK_INDEX;
    }
    newFilter.FilterMode  = CAN_FILTERMODE_IDMASK;
    newFilter.FilterScale = CAN_FILTERSCALE_16BIT;

    newFilter.FilterIdHigh         = filterExplicitId << 5; // must shift 11-bits to MSB of 16-bits
    newFilter.FilterIdLow          = 0;
    newFilter.FilterMaskIdHigh     = filterMask;
    newFilter.FilterMaskIdLow      = 0xFFFF; // block off second filter with all 1s
    newFilter.FilterFIFOAssignment = CAN_FILTER_FIFO0;

    newFilter.FilterActivation     = ENABLE;
    newFilter.SlaveStartFilterBank = CANf4xx::SECOND_FILTER_BANK_INDEX;

    if (HAL_CAN_ConfigFilter(&halCAN, &newFilter) != HAL_OK) {
        return CANStatus::ERROR;
    }
    return CANStatus::OK;
}

CAN::CANStatus CANf4xx::enableEmergencyFilter(uint32_t state) {
    CAN_FilterTypeDef emergencyFilter;

    if (halCAN.Instance == CAN1) {
        emergencyFilter.FilterBank = 1;
    } else {
        emergencyFilter.FilterBank = 1 + CANf4xx::SECOND_FILTER_BANK_INDEX;
    }

    emergencyFilter.FilterIdHigh     = 0x1000; // only 0001 (emergency code) allowed
    emergencyFilter.FilterIdLow      = 0;
    emergencyFilter.FilterMaskIdHigh = 0xF000; // 1111000000000000 Only looking for 4-bit code
    emergencyFilter.FilterMaskIdLow  = 0xFFFF; // block off second filter with all 1s

    emergencyFilter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    emergencyFilter.FilterMode           = CAN_FILTERMODE_IDMASK;
    emergencyFilter.FilterScale          = CAN_FILTERSCALE_16BIT;
    emergencyFilter.FilterActivation     = state;
    emergencyFilter.SlaveStartFilterBank = CANf4xx::SECOND_FILTER_BANK_INDEX;

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

} // namespace core::io
