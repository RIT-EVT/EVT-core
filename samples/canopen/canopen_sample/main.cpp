/**
 * This sample shows off CANopen support from EVT-core. This will
 * setup a CANopen node and attempt to make back and forth communication.
 *
 * This sample is intended to be run alongside canopen_rpdo.
 */

#include <core/io/CAN.hpp>
#include <core/io/UART.hpp>
#include <core/io/types/CANMessage.hpp>
#include <core/manager.hpp>
#include <core/utils/time.hpp>
#include <core/utils/types/FixedQueue.hpp>

#include <core/io/CANopen.hpp>

#include <co_core.h>
#include <co_if.h>
#include <co_tmr.h>

#include "TestCanNode.hpp"

namespace io   = core::io;
namespace dev  = core::dev;
namespace time = core::time;

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
void canInterrupt(io::CANMessage& message, void* priv) {
    core::types::FixedQueue<CANOPEN_QUEUE_SIZE, io::CANMessage>* queue =
        (core::types::FixedQueue<CANOPEN_QUEUE_SIZE, io::CANMessage>*) priv;
    if (queue != nullptr)
        queue->append(message);
}

int main() {
    // Initialize system
    core::platform::init();

    // Initialize the timer
    dev::Timer& timer = dev::getTimer<dev::MCUTimer::Timer2>(100);

    // UART for testing
    io::UART& uart = io::getUART<io::Pin::UART_TX, io::Pin::UART_RX>(9600);

    TestCanNode testCanNode;

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

    uint8_t lastValue = 0;
    while (1) {
        if (testCanNode.getSampleData() != lastValue) {
            uart.printf("Value of my number: %d\n\r", testCanNode.getSampleData());
            lastValue = testCanNode.getSampleData();
        }

        io::processCANopenNode(&canNode);
        // Wait for new data to come in
        time::wait(10);
    }
}
