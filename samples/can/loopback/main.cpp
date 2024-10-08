/**
 * Basic example of using CAN. In this situation the device is setup in
 * loopback so that the CAN messages can be sent and received to itself.
 *
 * @author Collin Bolles
 */
#include <core/io/CAN.hpp>
#include <core/manager.hpp>
#include <core/utils/time.hpp>

namespace io   = core::io;
namespace time = core::time;

int main() {
    // Initialize system
    core::platform::init();

    // Get CAN instance with loopback enabled
    io::CAN& can   = io::getCAN<io::Pin::PA_12, io::Pin::PA_11>(true);
    io::UART& uart = io::getUART<io::Pin::UART_TX, io::Pin::UART_RX>(9600);

    uint8_t payload[] = {0xDE, 0xAD, 0xBE, 0xBE, 0xEF, 0x00, 0x01, 0x02};
    io::CANMessage transmit_message(0b00010011110, 8, &payload[0], false);
    io::CANMessage received_message;

    io::CAN::CANStatus result;

    // Attempt to join the CAN network
    result = can.connect();

    //  can.addCANFilter(0, 0, 13);  //This would create a filter that allows all messages through
    can.addCANFilter(0b00000011010, 0b0000111111100000, 2);
    can.addCANFilter(0b00001010111, 0b0000111111100000, 3);
    can.enableEmergencyFilter(ENABLE);

    uart.printf("Starting CAN testing\r\n");

    if (result != io::CAN::CANStatus::OK) {
        uart.printf("Failed to connect to CAN network\r\n");
        return 1;
    }

    while (true) {
        result = can.transmit(transmit_message);
        if (result != io::CAN::CANStatus::OK) {
            uart.printf("Failed to transmit message\r\n");
            return 1;
        }

        result = can.receive(&received_message, false);
        if (result != io::CAN::CANStatus::OK) {
            uart.printf("Failed to receive message\r\n");
            continue;
        }

        if (received_message.getDataLength() == 0) {
            uart.printf("Message filtered out!");
        } else {
            uart.printf("Message received\r\n");
            uart.printf("Message id: %d \r\n", received_message.getId());
            uart.printf("Message length: %d\r\n", received_message.getDataLength());
            uart.printf("Message contents: ");

            uint8_t* message_payload = received_message.getPayload();
            for (int i = 0; i < received_message.getDataLength(); i++) {
                uart.printf("0x%02X ", message_payload[i]);
            }
        }
        uart.printf("\r\n\r\n");

        time::wait(2000);
    }

    return 0;
}