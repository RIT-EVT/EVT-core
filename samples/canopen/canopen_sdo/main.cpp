/**
 * This sample shows off CANopen support from EVT-core. This will
 * setup a CANopen node and attempt to make back and forth communication.
 */

#include <core/io/CAN.hpp>
#include <core/io/UART.hpp>
#include <core/io/types/CANMessage.hpp>
#include <core/manager.hpp>
#include <core/utils/time.hpp>
#include <core/utils/types/FixedQueue.hpp>

#include <core/io/CANopen.hpp>
#include <cstdio>

#include "SDOCanNode.hpp"

namespace io   = core::io;
namespace dev  = core::dev;
namespace time = core::time;

// create a can interrupt handler
void canInterrupt(io::CANMessage& message, void* priv) {
    auto* queue = (core::types::FixedQueue<CANOPEN_QUEUE_SIZE, io::CANMessage>*) priv;
    char messageString[50];

    uint8_t* data = message.getPayload();
    if (queue != nullptr)
        queue->append(message);
    for (int i = 0; i < message.getDataLength(); i++) {
        snprintf(&messageString[i * 5], 6, "0x%02X ", data[i]);
    }

    log::LOGGER.log(log::Logger::LogLevel::DEBUG,
                    "[CAN1] Got RAW message from %X of length %d with data: \r\n\t%s\r\n",
                    message.getId(),
                    message.getDataLength(),
                    messageString);
}

void SdoTransferCallback(CO_CSDO* csdo, uint16_t index, uint8_t sub, uint32_t code) {
    char messageString[50];
    if (code == 0) {
        /* read data is available in 'readValue' */
        snprintf(&messageString[0], 25, "Value transferred %x, %x\r\n", csdo->Tfer.Buf[0], csdo->Tfer.Buf[1]);
    } else {
        /* a timeout or abort is detected during SDO transfer  */
        snprintf(&messageString[0], 36, "SDO transfer callback don goofed 0x%x\r\n", code);
    }

    log::LOGGER.log(log::Logger::LogLevel::DEBUG, "SDO Transfer Operation: \r\n\t%s\r\n", messageString);
}

void SdoReceiveCallback(CO_CSDO* csdo, uint16_t index, uint8_t sub, uint32_t code) {
    char messageString[50];
    if (code == 0) {
        /* read data is available in 'readValue' */
        snprintf(&messageString[0], 25, "Value received %x, %x\r\n", csdo->Tfer.Buf[0], csdo->Tfer.Buf[1]);
    } else {
        /* a timeout or abort is detected during SDO transfer  */
        snprintf(&messageString[0], 36, "SDO receive callback don goofed 0x%x\r\n", code);
    }

    log::LOGGER.log(log::Logger::LogLevel::DEBUG, "SDO Receive Operation: \r\n\t%s\r\n", messageString);
}

int main() {
    // Initialize system
    core::platform::init();

    io::UART& uart = io::getUART<io::Pin::UART_TX, io::Pin::UART_RX>(9600);
    log::LOGGER.setUART(&uart);
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

    core::io::registerCallBack(SdoTransferCallback, &canNode);
    core::io::registerCallBack(SdoReceiveCallback, &canNode);

    ///////////////////////////////////////////////////////////////////////////
    // Main loop
    ///////////////////////////////////////////////////////////////////////////
    uint32_t lastUpdate1 = HAL_GetTick();
    uint32_t lastUpdate2 = HAL_GetTick();

    while (1) {
        if ((HAL_GetTick() - lastUpdate1) >= 1000) {        // If 1000ms have passed receive CAN message.
            testCanNode.receiveData();                      // Receive data from server
            lastUpdate1 = HAL_GetTick();                    // Set to current time.
        } else if ((HAL_GetTick() - lastUpdate2) >= 5000) { // If 5000ms have passed write CAN message.
            testCanNode.transferData();                     // Send data to server
            lastUpdate2 = HAL_GetTick();                    // Set to current time.
        }

        io::processCANopenNode(&canNode);
        // Wait for new data to come in
        time::wait(10);
    }
}