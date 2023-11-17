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

    // Reserve memory for the SDO buffers. We take the number of SDOs supported and
    // multiply it by the size of a single SDO Buffer to get the right buffer size.
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
