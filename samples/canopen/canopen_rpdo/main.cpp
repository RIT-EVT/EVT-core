/**
 * This sample shows off CANopen support from EVT-core. This will
 * setup a CANopen node and attempt to make back and forth communication.
 */
#include <stdint.h>

#include <EVT/io/ADC.hpp>
#include <EVT/io/CAN.hpp>
#include <EVT/io/UART.hpp>
#include <EVT/io/types/CANMessage.hpp>
#include <EVT/manager.hpp>
#include <EVT/utils/time.hpp>
#include <EVT/utils/types/FixedQueue.hpp>

#include <EVT/io/CANopen.hpp>

#include <co_core.h>
#include <co_if.h>
#include <co_tmr.h>

#include "RPDOCanNode.hpp"

namespace IO = EVT::core::IO;
namespace DEV = EVT::core::DEV;
namespace time = EVT::core::time;

///////////////////////////////////////////////////////////////////////////////
// EVT-core CAN callback and CAN setup. This will include logic to set
// aside CANopen messages into a specific queue
///////////////////////////////////////////////////////////////////////////////

/**
 * Interrupt handler to get CAN messages. A function pointer to this function
 * will be passed to the EVT-core CAN interface which will in turn call this
 * function each time a new CAN message comes in.
 *
 * NOTE: For this sample, every non-extended (so 11 bit CAN IDs) will be
 * assumed to be intended to be passed as a CANopen message.
 *
 * @param message[in] The passed in CAN message that was read.
 */

IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);

// create a can interrupt handler
void canInterrupt(IO::CANMessage& message, void* priv) {
    auto* queue = (EVT::core::types::FixedQueue<CANOPEN_QUEUE_SIZE, IO::CANMessage>*) priv;

    //print out raw received data
    uart.printf("Got RAW message from %X of length %d with data: ", message.getId(), message.getDataLength());
    uint8_t* data = message.getPayload();
    for (int i = 0; i < message.getDataLength(); i++) {
        uart.printf("%X ", *data);
        data++;
    }
    uart.printf("\r\n");

    if (queue != nullptr)
        queue->append(message);
}

void processError(CO_NODE* canNode) {
    switch (CONodeGetErr(canNode)) {
    case CO_ERR_NONE:
        uart.printf("no error");
        break;
    case CO_ERR_BAD_ARG:
        uart.printf("invalid argument");
        break;
    case CO_ERR_OBJ_NOT_FOUND:
        uart.printf("!< object not found in dictionary");
        break;
    case CO_ERR_OBJ_READ:
        uart.printf("error in reading an object entry");
        break;
    case CO_ERR_OBJ_WRITE:
        uart.printf("error in writing an object entry");
        break;
    case CO_ERR_OBJ_SIZE:
        uart.printf("read/write with wrong size to object");
        break;
    case CO_ERR_OBJ_INIT:
        uart.printf("error in initializing an object entry");
        break;

    case CO_ERR_OBJ_MAP_LEN:
        uart.printf("invalid mapping length");
        break;
    case CO_ERR_OBJ_MAP_TYPE:
        uart.printf("invalid mapping type");
        break;
    case CO_ERR_OBJ_ACC:
        uart.printf("unsupported access");
        break;
    case CO_ERR_OBJ_RANGE:
        uart.printf("value range of parameter exceeded");
        break;
    case CO_ERR_OBJ_INCOMPATIBLE:
        uart.printf("incompatible parameter value");
        break;

    case CO_ERR_DICT_INIT:
        uart.printf("error in initialization of dictionary");
        break;

    case CO_ERR_PARA_IDX:
        uart.printf("wrong index for parameter type");
        break;
    case CO_ERR_PARA_STORE:
        uart.printf("error during storing parameter");
        break;
    case CO_ERR_PARA_RESTORE:
        uart.printf("error during restoring parameter");
        break;
    case CO_ERR_PARA_LOAD:
        uart.printf("error during loading parameter");
        break;

    case CO_ERR_LSS_STORE:
        uart.printf("error during storing LSS configuration");
        break;
    case CO_ERR_LSS_LOAD:
        uart.printf("error during loading LSS configuration");
        break;

    case CO_ERR_CFG_1001_0:
        uart.printf("entry 1001:0 is bad/not existing");
        break;
    case CO_ERR_CFG_1003_0:
        uart.printf("entry 1003:0 is bad/not existing");
        break;
    case CO_ERR_CFG_1003_1:
        uart.printf("entry 1003:1 is bad/not existing");
        break;
    case CO_ERR_CFG_1005_0:
        uart.printf("entry 1005:0 is bad/not existing");
        break;
    case CO_ERR_CFG_1006_0:
        uart.printf("entry 1006:0 is bad/not existing");
        break;
    case CO_ERR_CFG_1010_0:
        uart.printf("entry 1010:0 is bad/not existing");
        break;
    case CO_ERR_CFG_1011_0:
        uart.printf("entry 1011:0 is bad/not existing");
        break;
    case CO_ERR_CFG_1014_0:
        uart.printf("entry 1014:0 is bad/not existing");
        break;
    case CO_ERR_CFG_1017_0:
        uart.printf("entry 1017:0 is bad/not existing");
        break;
    case CO_ERR_CFG_1016:
        uart.printf("entry in 1016 is bad/not existing");
        break;
    case CO_ERR_CFG_1018:
        uart.printf("entry in 1018 is bad/not existing");
        break;

    case CO_ERR_TMR_NO_ACT:
        uart.printf("no action available while creating");
        break;
    case CO_ERR_TMR_INSERT:
        uart.printf("error during insert action in tmr-list");
        break;
    case CO_ERR_TMR_CREATE:
        uart.printf("error during creating a timed action");
        break;
    case CO_ERR_TMR_DELETE:
        uart.printf("error during deleting a timed action");
        break;

    case CO_ERR_NMT_INIT:
        uart.printf("error during initializing NMT slave");
        break;
    case CO_ERR_NMT_APP_RESET:
        uart.printf("error in resetting application");
        break;
    case CO_ERR_NMT_COM_RESET:
        uart.printf("error in resetting communication");
        break;
    case CO_ERR_NMT_MODE:
        uart.printf("action not allowed in current NMT mode");
        break;

    case CO_ERR_EMCY_BAD_ROOT:
        uart.printf("error in emcy structure: member: Root");
        break;

    case CO_ERR_TPDO_COM_OBJ:
        uart.printf("config error in TPDO communication");
        break;
    case CO_ERR_TPDO_MAP_OBJ:
        uart.printf("config error in TPDO mapping");
        break;
    case CO_ERR_TPDO_OBJ_TRIGGER:
        uart.printf("error during trigger via an object");
        break;
    case CO_ERR_TPDO_NUM_TRIGGER:
        uart.printf("error during trigger via an PDO number");
        break;
    case CO_ERR_TPDO_INHIBIT:
        uart.printf("error during inhibit timer creation");
        break;
    case CO_ERR_TPDO_EVENT:
        uart.printf("error during event timer creation");
        break;

    case CO_ERR_RPDO_COM_OBJ:
        uart.printf("config error in RPDO communication");
        break;
    case CO_ERR_RPDO_MAP_OBJ:
        uart.printf("config error in RPDO mapping");
        break;
    case CO_ERR_SDO_SILENT:
        uart.printf("no SDO response (e.g. block transfer)");
        break;
    case CO_ERR_SDO_OFF:
        uart.printf("SDO client is disabled");
        break;
    case CO_ERR_SDO_BUSY:
        uart.printf("SDO client transfer is ongoing");
        break;
    case CO_ERR_SDO_ABORT:
        uart.printf("error in SDO request with ABORT resp.");
        break;
    case CO_ERR_SDO_READ:
        uart.printf("error during in SDO block reading");
        break;
    case CO_ERR_SDO_WRITE:
        uart.printf("error during in SDO block writing");
        break;
    case CO_ERR_SYNC_MSG:
        uart.printf("error during receive synchronous PDO");
        break;
    case CO_ERR_SYNC_RES:
        uart.printf("SYNC cycle is out of resolution");
        break;
    case CO_ERR_IF_CAN_INIT:
        uart.printf("error during initialization");
        break;
    case CO_ERR_IF_CAN_ENABLE:
        uart.printf("error during enabling CAN interface");
        break;
    case CO_ERR_IF_CAN_FLUSH_RX:
        uart.printf("error during flushing CAN RX interface");
        break;
    case CO_ERR_IF_CAN_FLUSH_TX:
        uart.printf("error during flushing CAN TX interface");
        break;
    case CO_ERR_IF_CAN_RESET:
        uart.printf("error during resetting CAN interface");
        break;
    case CO_ERR_IF_CAN_CLOSE:
        uart.printf("error during closing the CAN interface");
        break;
    case CO_ERR_IF_CAN_READ:
        uart.printf("error during reading from CAN interface");
        break;
    case CO_ERR_IF_CAN_SEND:
        uart.printf("error during sending to CAN interface");
        break;
    case CO_ERR_IF_TIMER_INIT:
        uart.printf("error during initializing timer");
        break;
    case CO_ERR_IF_TIMER_UPDATE:
        uart.printf("error during updating timer");
        break;
    case CO_ERR_IF_TIMER_RELOAD:
        uart.printf("error during reloading timer");
        break;
    case CO_ERR_IF_TIMER_DELAY:
        uart.printf("error during getting remaining ticks");
        break;
    case CO_ERR_IF_TIMER_STOP:
        uart.printf("error during stopping timer");
        break;
    case CO_ERR_IF_TIMER_START:
        uart.printf("error during starting timer");
        break;
    case CO_ERR_IF_NVM_INIT:
        uart.printf("error during initializing NVM");
        break;
    case CO_ERR_IF_NVM_READ:
        uart.printf("error during reading NVM");
        break;
    case CO_ERR_IF_NVM_WRITE:
        uart.printf("error during writing NVM");
        break;
    case CO_ERR_SIG_INIT:
        uart.printf("error during initializing CAN signals");
        break;
    case CO_ERR_SIG_CREATE:
        uart.printf("error during creating a needed signal");
        break;
    case CO_ERR_MSG_INIT:
        uart.printf("error during message initialization");
        break;
    case CO_ERR_MSG_CREATE:
        uart.printf("error during creating a message");
        break;
    case CO_ERR_MSG_READ:
        uart.printf("error during reading a message");
        break;
    case CO_ERR_TYPE_INIT:
        uart.printf("error during type initialization");
        break;
    case CO_ERR_TYPE_RD:
        uart.printf("error during reading type");
        break;
    case CO_ERR_TYPE_WR:
        uart.printf("error during writing type");
        break;
    case CO_ERR_TYPE_RESET:
        uart.printf("error during reset type");
        break;
    }
    uart.printf("\n\r");
}

int main() {
    // Initialize system
    EVT::core::platform::init();

    // Will store CANopen messages that will be populated by the EVT-core CAN
    // interrupt
    EVT::core::types::FixedQueue<CANOPEN_QUEUE_SIZE, IO::CANMessage> canOpenQueue;

    // Initialize CAN, add an IRQ which will add messages to the queue above
    IO::CAN& can = IO::getCAN<IO::Pin::PA_12, IO::Pin::PA_11>();
    can.addIRQHandler(canInterrupt, reinterpret_cast<void*>(&canOpenQueue));

    // Initialize the timer
    DEV::Timer& timer = DEV::getTimer<DEV::MCUTimer::Timer2>(100);
    timer.stopTimer();

    //create the RPDO node
    RPDOCanNode testCanNode;

    // Reserved memory for CANopen stack usage
    uint8_t sdoBuffer[CO_SSDO_N * CO_SDO_BUF_BYTE];
    CO_TMR_MEM appTmrMem[16];

    // Attempt to join the CAN network
    IO::CAN::CANStatus result = can.connect();

    //test that the board is connected to the can network
    if (result != IO::CAN::CANStatus::OK) {
        uart.printf("Failed to connect to CAN network\r\n");
        return 1;
    }

    ///////////////////////////////////////////////////////////////////////////
    // Setup CAN configuration, this handles making drivers, applying settings.
    // And generally creating the CANopen stack node which is the interface
    // between the application (the code we write) and the physical CAN network
    ///////////////////////////////////////////////////////////////////////////
    // Make drivers
    CO_IF_DRV canStackDriver;

    CO_IF_CAN_DRV canDriver;
    CO_IF_TIMER_DRV timerDriver;
    CO_IF_NVM_DRV nvmDriver;

    IO::getCANopenCANDriver(&can, &canOpenQueue, &canDriver);
    IO::getCANopenTimerDriver(&timer, &timerDriver);
    IO::getCANopenNVMDriver(&nvmDriver);

    canStackDriver.Can = &canDriver;
    canStackDriver.Timer = &timerDriver;
    canStackDriver.Nvm = &nvmDriver;

    //setup CANopen Node
    CO_NODE_SPEC canSpec = {
        .NodeId = RPDOCanNode::NODE_ID,
        .Baudrate = IO::CAN::DEFAULT_BAUD,
        .Dict = testCanNode.getObjectDictionary(),
        .DictLen = testCanNode.getNumElements(),
        .EmcyCode = NULL,
        .TmrMem = appTmrMem,
        .TmrNum = 16,
        .TmrFreq = 100,
        .Drv = &canStackDriver,
        .SdoBuf = reinterpret_cast<uint8_t*>(&sdoBuffer[0]),
    };

    CO_NODE canNode;

    CONodeInit(&canNode, &canSpec);
    CONodeStart(&canNode);
    CONmtSetMode(&canNode.Nmt, CO_OPERATIONAL);

    time::wait(500);

    //print any CANopen errors
    uart.printf("Error: %d\r\n", CONodeGetErr(&canNode));

    uint8_t lastVal1 = 0;
    uint16_t lastVal2 = 0;
    while (1) {
        //Print new value when changed over CAN
        if (lastVal1 != testCanNode.getSampleDataA() || lastVal2 != testCanNode.getSampleDataB()) {
            lastVal1 = testCanNode.getSampleDataA();
            lastVal2 = testCanNode.getSampleDataB();
            uart.printf("Current value: %X, %X\r\n", lastVal1, lastVal2);
        }

        //        processError(&canNode);

        // Process incoming CAN messages
        CONodeProcess(&canNode);
        // Update the state of timer based events
        COTmrService(&canNode.Tmr);
        // Handle executing timer events that have elapsed
        COTmrProcess(&canNode.Tmr);
        // Wait for new data to come in
        time::wait(10);
    }
}
