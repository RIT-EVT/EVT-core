/**
 * Example of CAN communication using dual CAN interfaces. The two interfaces need
 * to be connected to a CAN network. Both interfaces also can be connected to the
 * same network as a loop back with a CAN transceiver. ONLY WORKS ON STM32F4xx!
 */

#include <stdint.h>
#include <string>

#include <core/io/CAN.hpp>
#include <core/manager.hpp>
#include <core/utils/log.hpp>
#include <core/utils/time.hpp>

namespace io   = core::io;
namespace time = core::time;
namespace log  = core::log;

/**
 * CAN 1 interrupt handler to print out received CAN messages.
 */
void can1IRQHandler(io::CANMessage& message, void* priv) {
    char messageString[50];
    uint8_t* message_payload = message.getPayload();
    for (int i = 0; i < message.getDataLength(); i++) {
        snprintf(&messageString[i * 5], 6, "0x%02X ", message_payload[i]);
    }

    log::LOGGER.log(log::Logger::LogLevel::DEBUG,
                    "[CAN1] Message received from %d with %d bytes containing: \r\n\t%s\r\n",
                    message.getId(),
                    message.getDataLength(),
                    messageString);
}

/**
 * CAN 2 interrupt handler to print out received CAN messages.
 */
void can2IRQHandler(io::CANMessage& message, void* priv) {
    char messageString[50];
    uint8_t* message_payload = message.getPayload();
    for (int i = 0; i < message.getDataLength(); i++) {
        snprintf(&messageString[i * 5], 6, "0x%02X ", message_payload[i]);
    }

    log::LOGGER.log(log::Logger::LogLevel::DEBUG,
                    "[CAN2] Message received from %d with %d bytes containing: \r\n\t%s\r\n",
                    message.getId(),
                    message.getDataLength(),
                    messageString);
}

int main() {
    // Initialize system
    core::platform::init();
    io::UART& uart = io::getUART<io::Pin::UART_TX, io::Pin::UART_RX>(9600);
    log::LOGGER.setUART(&uart);
    log::LOGGER.setLogLevel(log::Logger::LogLevel::DEBUG);

    // Get CAN instances

    // CAN 1
    io::CAN& can1 = io::getCAN<io::Pin::PA_12, io::Pin::PA_11>();
    // io::CAN& can1 = io::getCAN<io::Pin::PB_9, io::Pin::PB_8>();

    // CAN 2
    // io::CAN& can2 = io::getCAN<io::Pin::PB_6, io::Pin::PB_5>();
    io::CAN& can2 = io::getCAN<io::Pin::PB_13, io::Pin::PB_12>();

    // Setup interrupt handlers
    can1.addIRQHandler(can1IRQHandler, nullptr);
    can2.addIRQHandler(can2IRQHandler, nullptr);

    uint8_t can1Count = 0;
    uint8_t can2Count = 0;

    // CAN message that will be sent
    uint8_t payload1[] = {0xDE, 0xAD, 0xBE, 0xBE, 0xEF, 0x09, 0x08, 0x07};
    uint8_t payload2[] = {0xBE, 0xEF, 0xBE, 0xDE, 0xAD, 0x00, 0x01, 0x02};
    io::CANMessage can1TransmitMessage(1, 8, &payload1[0], false);
    io::CANMessage can2TransmitMessage(2, 8, &payload2[0], false);

    log::LOGGER.log(log::Logger::LogLevel::INFO, "Starting CAN testing\r\n");

    io::CAN::CANStatus result;

    // Attempt to join the CAN network.
    result = can1.connect();
    if (result != io::CAN::CANStatus::OK) {
        log::LOGGER.log(log::Logger::LogLevel::ERROR, "[CAN1] Failed to connect to the CAN network\r\n");
        return 1;
    }

    result = can2.connect();
    if (result != io::CAN::CANStatus::OK) {
        log::LOGGER.log(log::Logger::LogLevel::ERROR, "[CAN2] Failed to connect to the CAN network\r\n");
        return 1;
    }

    while (true) {
        // Update the payload of the CAN messages with the counts.
        payload1[7] = can1Count;
        payload2[5] = can2Count;

        can1TransmitMessage.setPayload(payload1);
        can2TransmitMessage.setPayload(payload2);

        // Send both messages on the two different buses.
        result = can1.transmit(can1TransmitMessage);
        if (result != io::CAN::CANStatus::OK) {
            log::LOGGER.log(log::Logger::LogLevel::ERROR, "[CAN1] Failed to transmit message\r\n");
            return 1;
        }

        result = can2.transmit(can2TransmitMessage);
        if (result != io::CAN::CANStatus::OK) {
            log::LOGGER.log(log::Logger::LogLevel::ERROR, "[CAN2] Failed to transmit message\r\n");
            return 1;
        }

        // Update the counts.
        can1Count++;
        can2Count--;

        time::wait(1000);
    }

    return 0;
}
