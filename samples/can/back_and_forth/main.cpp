/**
 * Example of CAN communication between two devices. The two devices need
 * to be on a CAN network. The other device can be running this sample code
 * as well.
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
    IO::CAN& can = IO::getCAN<IO::Pin::PA_12, IO::Pin::PA_11>();
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);

    // CAN message that will be sent
    uint8_t payload[] = { 0xDE, 0xAD, 0xBE, 0xBE, 0xEF, 0x00, 0x01, 0x02 };
    IO::CANMessage transmit_message(1, 8, &payload[0]);
    IO::CANMessage received_message;

    uart.printf("Starting CAN testing\r\n");

    while (true) {
        can.transmit(transmit_message);
        can.receive(&received_message, true);

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