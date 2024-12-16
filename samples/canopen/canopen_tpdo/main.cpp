/**
 * This sample shows off CANopen support from EVT-core. This will
 * setup a CANopen node and attempt to make back and forth communication.
 */

#include <string>
#include <core/io/CAN.hpp>
#include <core/io/UART.hpp>
#include <core/io/types/CANMessage.hpp>
#include <core/manager.hpp>
#include <core/utils/time.hpp>
#include <core/utils/types/FixedQueue.hpp>
#include <core/utils/log.hpp>

#include <core/io/CANopen.hpp>

#include "TPDOCanNode.hpp"

namespace io   = core::io;
namespace dev  = core::dev;
namespace time = core::time;
namespace log  = core::log;

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


// create a can interrupt handler
void canInterrupt(io::CANMessage& message, void* priv) {
    auto* queue = (core::types::FixedQueue<CANOPEN_QUEUE_SIZE, io::CANMessage>*) priv;
    char messageString[50];

    // print out raw received data
    snprintf(&messageString[5], 6, "Got RAW message from %X of length %d with data: ", message.getId(), message.getDataLength());
    uint8_t* data = message.getPayload();
    for (int i = 0; i < message.getDataLength(); i++) {
        snprintf(&messageString[i * 5], 1, "%X ", *data);
        data++;
    }
    log::LOGGER.log(log::Logger::LogLevel::INFO,"\r\n\t%s\r\n", messageString);

    if (queue != nullptr)
        queue->append(message);
}

// setup a TPDO event handler to print the raw TPDO message when sending
extern "C" void COPdoTransmit(CO_IF_FRM* frm) {
    char messageString[50];
    snprintf(&messageString[5], 6, "Sending PDO as 0x%X with length %d and data: ", frm->Identifier, frm->DLC);

    uint8_t* data = frm->Data;
    for (int i = 0; i < frm->DLC; i++) {
        snprintf(&messageString[i * 5], 1, "%X ", *data);
        data++;
    }
    log::LOGGER.log(log::Logger::LogLevel::INFO,"\r\n\t%s\r\n", messageString);
}

int main() {
    // Initialize system
    core::platform::init();

    io::UART& uart = io::getUART<io::Pin::UART_TX, io::Pin::UART_RX>(9600);

    // create the TPDO node
    TPDOCanNode testCanNode;

    dev::Timer& timer = dev::getTimer<dev::MCUTimer::Timer2>(100);

    ///////////////////////////////////////////////////////////////////////////
    // Setup CAN configuration, this handles making drivers, applying settings.
    // And generally creating the CANopen stack node which is the interface
    // between the application (the code we write) and the physical CAN network
    ///////////////////////////////////////////////////////////////////////////

    // Will store CANopen messages that will be populated by the EVT-core CAN
    // interrupt
    core::types::FixedQueue<CANOPEN_QUEUE_SIZE, io::CANMessage> canOpenQueue;

    // Initialize CAN, add an IRQ which will add messages to the queue above
    io::CAN& can = io::getCAN<io::Pin::PA_12, io::Pin::PA_11>();
    can.addIRQHandler(canInterrupt, reinterpret_cast<void*>(&canOpenQueue));

    // Reserved memory for CANopen stack usage
    uint8_t sdoBuffer[CO_SSDO_N * CO_SDO_BUF_BYTE];
    CO_TMR_MEM appTmrMem[16];

    // Reserve driver variables
    CO_IF_DRV canStackDriver;

    CO_IF_CAN_DRV canDriver;
    CO_IF_TIMER_DRV timerDriver;
    CO_IF_NVM_DRV nvmDriver;

    CO_NODE canNode;

    // Attempt to join the CAN network
    io::CAN::CANStatus result = can.connect();

    // test that the board is connected to the can network
    if (result != io::CAN::CANStatus::OK) {
        uart.printf("Failed to connect to CAN network\r\n");
        return 1;
    }

    // Initialize all the CANOpen drivers.
    io::initializeCANopenDriver(&canOpenQueue, &can, &timer, &canStackDriver, &nvmDriver, &timerDriver, &canDriver);

    // Initialize the CANOpen node we are using.
    io::initializeCANopenNode(&canNode, &testCanNode, &canStackDriver, sdoBuffer, appTmrMem);

    // Set the node to operational mode
    CONmtSetMode(&canNode.Nmt, CO_OPERATIONAL);

    time::wait(500);

    // print any CANopen errors
    uart.printf("Error: %d\r\n", CONodeGetErr(&canNode));

    ///////////////////////////////////////////////////////////////////////////
    // Main loop
    ///////////////////////////////////////////////////////////////////////////

    uint8_t lastVal1  = 0;
    uint16_t lastVal2 = 0;
    while (1) {
        // increment node values
        testCanNode.update();
        if (lastVal1 != testCanNode.getSampleDataA() || lastVal2 != testCanNode.getSampleDataB()) {
            lastVal1 = testCanNode.getSampleDataA();
            lastVal2 = testCanNode.getSampleDataB();
            uart.printf("Current value: %X, %X\r\n", lastVal1, lastVal2);
        }

        io::processCANopenNode(&canNode);
        // Wait for new data to come in
        time::wait(50);
    }
}
