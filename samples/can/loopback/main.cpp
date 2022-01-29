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

    uint8_t payloadA[] = {0xDE, 0xAD, 0xBE, 0xBE, 0xEF, 0x00, 0x01, 0x02};
    IO::CANMessage transmit_messageA(1, 8, &payloadA[0], false);
    IO::CANMessage received_messageA;

    uint8_t payloadB[] = {0xAA, 0xBB, 0xCC, 0xCC, 0xDD, 0xEE, 0xFF, 0x00};
    IO::CANMessage transmit_messageB(0b01100000011, 8, &payloadB[0], false);
    IO::CANMessage received_messageB;

    uart.printf("Starting CAN testing\r\n");

    while (true) {
        can.transmit(transmit_messageA);
        can.receive(&received_messageA, false);

        if(received_messageA.getDataLength() == 0) {
            uart.printf("MessageA filtered out!");
        } else {
            uart.printf("Message A received\r\n");
            uart.printf("Message id: %d \r\n", received_messageA.getId());
            uart.printf("Message length: %d\r\n", received_messageA.getDataLength());
            uart.printf("Message contents: ");

            uint8_t* message_payloadA = received_messageA.getPayload();
            for (int i = 0; i < received_messageA.getDataLength(); i++) {
                uart.printf("0x%02X ", message_payloadA[i]);
            }
        }
        uart.printf("\r\n\r\n");

        time::wait(1000);

        can.transmit(transmit_messageB);
        can.receive(&received_messageB, false);

        if(received_messageB.getDataLength() == 0) {
            uart.printf("MessageB filtered out!");
        } else {
            uart.printf("Message B received\r\n");
            uart.printf("Message id: %d \r\n", received_messageB.getId());
            uart.printf("Message length: %d\r\n", received_messageB.getDataLength());
            uart.printf("Message contents: ");

            uint8_t* message_payloadB = received_messageB.getPayload();
            for (int i = 0; i < received_messageB.getDataLength(); i++) {
                uart.printf("0x%02X ", message_payloadB[i]);
            }
        }
        uart.printf("\r\n\r\n");

        time::wait(1000);
    }

    return 0;
}
