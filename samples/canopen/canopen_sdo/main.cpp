/**
 * This sample shows the CANopen SDO feature. This will
 * set up the SDO client and will send and receive data
 * from the server node.
 *
 * This sample is intended to be run alongside canopen_tpdo.
 */

#include <core/io/CAN.hpp>
#include <core/io/UART.hpp>
#include <core/io/types/CANMessage.hpp>
#include <core/manager.hpp>
#include <core/utils/log.hpp>
#include <core/utils/time.hpp>
#include <core/utils/types/FixedQueue.hpp>
#include <cstring>

#include <core/io/CANopen.hpp>
#include <cstdio>

#include "SDOCanNode.hpp"

namespace io   = core::io;
namespace dev  = core::dev;
namespace time = core::time;
namespace log  = core::log;

io::UART* uart;

// Create a can interrupt handler
void canInterrupt(io::CANMessage& message, void* priv) {
    auto* queue = (core::types::FixedQueue<CANOPEN_QUEUE_SIZE, io::CANMessage>*) priv;
    char messageString[100];

    uint8_t* data = message.getPayload();
    if (queue != nullptr) {
        queue->append(message);
    }

    for (int i = 0; i < message.getDataLength(); i++) {
        snprintf(&messageString[strlen(messageString)], 6, "0x%02X ", data[i]);
    }

#ifdef EVT_CORE_LOG_ENABLE
    log::LOGGER.log(log::Logger::LogLevel::DEBUG,
                    "[CAN1] Got RAW message from %X of length %d with data: \r\n\t%s\r\n",
                    message.getId(),
                    message.getDataLength(),
                    messageString);
#endif
}

void SdoTransferCallback(CO_CSDO* csdo, uint32_t entry, uint32_t code, void* context) {
    char messageString[50];
    uint8_t* transferBuff = static_cast<uint8_t*>(context);
    if (code == 0) {
        /* read data is available */
        snprintf(&messageString[0], 25, "Value transferred %X\r\n", *transferBuff);
    } else {
        /* a timeout or abort is detected during SDO transfer  */
        snprintf(&messageString[0], 45, "SDO transfer callback don goofed %X\r\n", code);
    }

    log::LOGGER.log(log::Logger::LogLevel::DEBUG, "SDO Transfer Operation: \r\n\t%s\r\n", messageString);
}

void SdoReceiveCallback(CO_CSDO* csdo, uint32_t entry, uint32_t code, void* context) {
    char messageString[50];
    uint8_t* receiveBuff = static_cast<uint8_t*>(context);
    if (code == 0) {
        /* read data is available */
        snprintf(&messageString[0], 25, "Value received %X\r\n", *receiveBuff);
    } else {
        /* a timeout or abort is detected during SDO receive  */
        snprintf(&messageString[0], 45, "SDO receive callback don goofed %X\r\n", code);
    }

    log::LOGGER.log(log::Logger::LogLevel::DEBUG, "SDO Receive Operation: \r\n\t%s\r\n", messageString);
}

int main() {
    // Initialize system
    core::platform::init();

    uart = &io::getUART<io::Pin::UART_TX, io::Pin::UART_RX>(9600);
    log::LOGGER.setUART(uart);
    log::LOGGER.setLogLevel(log::Logger::LogLevel::DEBUG);

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

    // create the SDO node
    SDOCanNode testCanNode(canNode);

    // Attempt to join the CAN network
    io::CAN::CANStatus result = can.connect();

    // test that the board is connected to the can network
    if (result != io::CAN::CANStatus::OK) {
        uart->printf("Failed to connect to CAN network\r\n");
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
    uart->printf("Error: %d\r\n", CONodeGetErr(&canNode));

    ///////////////////////////////////////////////////////////////////////////
    // Main loop
    ///////////////////////////////////////////////////////////////////////////
    uint32_t lastUpdate1 = HAL_GetTick();
    uint32_t lastUpdate2 = HAL_GetTick();
    uint8_t sampleData = 0;

    while (1) {
        if ((HAL_GetTick() - lastUpdate1) >= 1000) {                 // If 1000ms have passed receive CAN message.
            testCanNode.receiveData(SdoReceiveCallback, &sampleData);   // Receive data from server
            lastUpdate1 = HAL_GetTick();                             // Set to current time.
        } else if ((HAL_GetTick() - lastUpdate2) >= 5000) {          // If 5000ms have passed write CAN message.
            testCanNode.transferData(SdoTransferCallback, &canNode); // Send data to server
            lastUpdate2 = HAL_GetTick();                             // Set to current time.
        }

        io::processCANopenNode(&canNode);
        // Wait for new data to come in
        time::wait(10);
    }
}