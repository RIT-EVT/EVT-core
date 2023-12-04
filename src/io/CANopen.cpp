#include <EVT/io/CANopen.hpp>
#include <EVT/io/types/CANMessage.hpp>
#include <EVT/utils/types/FixedQueue.hpp>

#include <EVT/dev/RTC.hpp>

#include "EVT/io/CANDevice.hpp"
#include <stdint.h>

#define MAX_SIZE 64

/*
 * Empty namespace to contain "global" variables. These will be used within
 * the driver implementations.
 */
namespace {
EVT::core::IO::CAN* globalCAN;
// Temporary values for testing CANopen without actual timer
EVT::core::DEV::Timer* globalTimer;

/** Counts the number of interrupts that have taken place */
uint32_t timerCounter = 0;
/** The target value for the counter */
uint32_t counterTarget = 0;

/* only return timer updates while timer is running - fixes hard fault in COTmrService*/
bool timerRunning = false;

// Temporary "storage" to allow the NVM to work, do not use as actual NVM
uint8_t testerStorage[MAX_SIZE];

// Queue that stores the CAN messages to send to the CANopen parser
EVT::core::types::FixedQueue<CANOPEN_QUEUE_SIZE, EVT::core::IO::CANMessage>* canQueue;
}// namespace

///////////////////////////////////////////////////////////////////////////////
// Forward declarations of CANopen stack CAN functions
///////////////////////////////////////////////////////////////////////////////
static void canInit(void);
static void canEnable(uint32_t baudrate);
static int16_t canSend(CO_IF_FRM* frm);
static int16_t canRead(CO_IF_FRM* frm);
static void canReset(void);
static void canClose(void);

///////////////////////////////////////////////////////////////////////////////
// Forward declarations of CANopen stack timer functions
///////////////////////////////////////////////////////////////////////////////
static void timerInit(uint32_t freq);
static void timerReload(uint32_t reload);
static void timerStart(void);
static uint8_t timerUpdate(void);
static uint32_t timerDelay(void);
static void timerStop(void);

///////////////////////////////////////////////////////////////////////////////
// Forward declarations of CANopen stack NVM functions
///////////////////////////////////////////////////////////////////////////////
static void nvmInit(void);
static uint32_t nvmRead(uint32_t start, uint8_t* buffer, uint32_t size);
static uint32_t nvmWrite(uint32_t start, uint8_t* buffer, uint32_t size);

namespace EVT::core::IO {
void getCANopenCANDriver(IO::CAN* canInf,
                         EVT::core::types::FixedQueue<CANOPEN_QUEUE_SIZE, IO::CANMessage>* messageQueue,
                         CO_IF_CAN_DRV* canDriver) {
    globalCAN = canInf;
    canQueue = messageQueue;
    canDriver->Init = canInit;
    canDriver->Enable = canEnable;
    canDriver->Read = canRead;
    canDriver->Send = canSend;
    canDriver->Reset = canReset;
    canDriver->Close = canClose;
}

void getCANopenTimerDriver(DEV::Timer* timerIntf, CO_IF_TIMER_DRV* timerDriver) {
    globalTimer = timerIntf;

    timerDriver->Init = timerInit;
    timerDriver->Reload = timerReload;
    timerDriver->Delay = timerDelay;
    timerDriver->Stop = timerStop;
    timerDriver->Start = timerStart;
    timerDriver->Update = timerUpdate;
}

void getCANopenNVMDriver(CO_IF_NVM_DRV* nvmDriver) {
    nvmDriver->Init = nvmInit;
    nvmDriver->Read = nvmRead;
    nvmDriver->Write = nvmWrite;
}

void initializeCANopenDriver(types::FixedQueue<CANOPEN_QUEUE_SIZE, IO::CANMessage>* canOpenQueue, IO::CAN* can, EVT::core::DEV::Timer* timer, CO_IF_DRV* canStackDriver, CO_IF_NVM_DRV* nvmDriver, CO_IF_TIMER_DRV* timerDriver, CO_IF_CAN_DRV* canDriver) { IO::getCANopenCANDriver(can, canOpenQueue, canDriver); IO::getCANopenTimerDriver(timer, timerDriver); IO::getCANopenNVMDriver(nvmDriver);

    canStackDriver->Can = canDriver;
    canStackDriver->Timer = timerDriver;
    canStackDriver->Nvm = nvmDriver;
}

void initializeCANopenNode(CO_NODE* canNode, CANDevice* canDevice, CO_IF_DRV* canStackDriver, uint8_t sdoBuffer[CO_SSDO_N * CO_SDO_BUF_BYTE], CO_TMR_MEM appTmrMem[16]) {
    //setup CANopen Node
    CO_NODE_SPEC canSpec = {
        .NodeId = canDevice->getNodeID(),
        .Baudrate = IO::CAN::DEFAULT_BAUD,
        .Dict = canDevice->getObjectDictionary(),
        .DictLen = canDevice->getNumElements(),
        .EmcyCode = NULL,
        .TmrMem = appTmrMem,
        .TmrNum = 16,
        .TmrFreq = 100,
        .Drv = canStackDriver,
        .SdoBuf = reinterpret_cast<uint8_t*>(&sdoBuffer[0]),
    };

    CONodeInit(canNode, &canSpec);
    CONodeStart(canNode);
}

void processCANopenNode(CO_NODE* canNode) {
    // Process incoming CAN messages
    CONodeProcess(canNode);
    // Update the state of timer based events
    COTmrService(&canNode->Tmr);
    // Handle executing timer events that have elapsed
    COTmrProcess(&canNode->Tmr);
}

}// namespace EVT::core::IO

///////////////////////////////////////////////////////////////////////////////
// Implementation of CANopen stack CAN drivers
///////////////////////////////////////////////////////////////////////////////
/**
 * Initialize the CAN driver. This doesn't do anything since the CAN interface
 * should be passed into `getCANopenDriver` pre-initialized.
 */
static void canInit(void) {
}

/**
 * Enable the CAN driver at the specific baudrate. Again this doesn't
 * do anything since the CAN interface will already be setup.
 */
static void canEnable(uint32_t baudrate) {
    // TODO: Should have the ability to reset the CAN driver
}

/**
 * Send a CAN message. This will convert the CANopen stack CAN message
 * format into the EVT::core::IO::CANMessage
 *
 * @param frm[in] The message to send over cat
 * @return sizeof(CO_IF_FRM) on success, ((int16_t)-1u) on failure
 */
static int16_t canSend(CO_IF_FRM* frm) {
    EVT::core::IO::CANMessage message(frm->Identifier, frm->DLC, frm->Data, false);
    globalCAN->transmit(message);

    return sizeof(CO_IF_FRM);
}

/**
 * Read in a CAN message. This will convert from the EVT::core::IO::CANMessage
 * into the CANopen stack format.
 *
 * @param frm[out] The message to populate with CAN data
 * @return sizeof(CO_IF_FMR) on success, ((int16_t) 0) on failure
 */
static int16_t canRead(CO_IF_FRM* frm) {
    EVT::core::IO::CANMessage message;

    // No message
    if (!canQueue->pop(&message))
        return ((int16_t) 0);// This should be 0 according to CANopen's COIfCanRead

    frm->Identifier = message.getId();
    frm->DLC = message.getDataLength();
    // Copy contents into payload buffer
    for (int i = 0; i < message.getDataLength(); i++) {
        frm->Data[i] = message.getPayload()[i];
    }

    return sizeof(CO_IF_FRM);
}

/**
 * Reset the CAN interface. This does nothing at the moment.
 */
static void canReset(void) {
}

/**
 * Close the CAN connection. This does nothing at the moment.
 */
static void canClose(void) {
}

///////////////////////////////////////////////////////////////////////////////
// Implementations of CANopen stack timer functions
///////////////////////////////////////////////////////////////////////////////

/**
 * Interrupt handler for the timer, updates that the timer has gone off
 */
void timerHandler(void* halTim) {
    timerCounter++;
}

/**
 * Initiailize the timer driver.
 */
static void timerInit(uint32_t freq) {
    timerCounter = 0;
    globalTimer->setPeriod(10);
}

static void timerReload(uint32_t reload) {
    globalTimer->stopTimer();
    globalTimer->setPeriod(10);
    globalTimer->startTimer(timerHandler);
    timerCounter = 0;
    timerRunning = true;
    counterTarget = reload;
}

/**
 * Start the "timer" running
 */
static void timerStart(void) {
    globalTimer->startTimer(timerHandler);
    timerRunning = true;
    timerCounter = 0;
}

/**
 * Return true if the timer has gone off
 */
static uint8_t timerUpdate(void) {
    int result = timerCounter >= counterTarget && timerRunning ? 1 : 0;
    return result;
}

/**
 * Get the difference between the current value and the target value
 */
static uint32_t timerDelay(void) {
    return timerCounter > counterTarget ? 0 : counterTarget - timerCounter;
}

/**
 * Stop the timer, currently does nothing.
 */
static void timerStop(void) {
    globalTimer->stopTimer();
    timerRunning = false;
    timerCounter = 0;
}

///////////////////////////////////////////////////////////////////////////////
// Implementation of CANopen stack NVM functions
///////////////////////////////////////////////////////////////////////////////
/**
 * Initialize the NVM driver, does nothing
 */
static void nvmInit(void) {
}

/**
 * Read from the temporary buffer
 */
static uint32_t nvmRead(uint32_t start, uint8_t* buffer, uint32_t size) {
    uint32_t bytesRead = 0;

    for (unsigned int i = 0; i < size && i + start < MAX_SIZE; i++) {
        buffer[i] = testerStorage[i + start];
        bytesRead++;
    }

    return bytesRead;
}

/**
 * Write to the temporary buffer
 */
static uint32_t nvmWrite(uint32_t start, uint8_t* buffer, uint32_t size) {
    uint32_t bytesWrote = 0;

    for (unsigned int i = 0; i < size && i + start < MAX_SIZE; i++) {
        testerStorage[i + start] = buffer[i];
        bytesWrote++;
    }
    return bytesWrote;
}
