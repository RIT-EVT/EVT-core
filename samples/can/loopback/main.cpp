/**
 * Basic example of using CAN. In this situation the device is setup in
 * loopback so that the CAN messages can be sent and received to itself.
 *
 * @author Collin Bolles
 */
#include <EVT/io/CAN.hpp>
#include <EVT/io/manager.hpp>
#include <EVT/utils/time.hpp>

namespace IO = EVT::core::IO;
namespace time = EVT::core::time;

int main() {
    // Initialize system
    IO::init();

    // Get CAN instance with loopback enabled
    IO::CAN& can = IO::getCAN<IO::Pin::PA_12, IO::Pin::PA_11>(true);
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);

    uint8_t payload[] = {0xDE, 0xAD, 0xBE, 0xBE, 0xEF, 0x00, 0x01, 0x02};
    IO::CANMessage transmit_message(1, 8, &payload[0], false);
    IO::CANMessage received_message;

    uart.printf("Starting CAN testing\r\n");

    IO::CAN::CANStatus result;

    // Attempt to join the CAN network
    result = can.connect();

    if(result != IO::CAN::CANStatus::OK) {
        uart.printf("Failed to connect to CAN network\r\n");
        return 1;
    }

    while (true) {
        result = can.transmit(transmit_message);
        if (result != IO::CAN::CANStatus::OK) {
            uart.printf("Failed to transmit message\r\n");
            return 1;
        }

        result = can.receive(&received_message, true);
        if (result != IO::CAN::CANStatus::OK) {
            uart.printf("Failed to receive message\r\n");
            return 1;
        }

        uart.printf("Message received\r\n");
        uart.printf("Message id: %d \r\n", received_message.getId());
        uart.printf("Message length: %d\r\n", received_message.getDataLength());
        uart.printf("Message contents: ");

        uint8_t* message_payload = received_message.getPayload();
        for (int i = 0; i < received_message.getDataLength(); i++) {
            uart.printf("0x%02X ", message_payload[i]);
        }
        uart.printf("\r\n\r\n");

        time::wait(1000);
    }

    return 0;
}
