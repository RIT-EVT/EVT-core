/**
 * Example of CAN communication between two devices. The two devices need
 * to be on a CAN network. The other device can be running this sample code
 * as well.
 *
 * @author Collin Bolles
 */
#include <core/io/CAN.hpp>
#include <core/manager.hpp>
#include <core/utils/time.hpp>

namespace io   = core::io;
namespace time = core::time;

void canIRQHandler(io::CANMessage& message, void* priv) {
    io::UART* uart = (io::UART*) priv;
//    uart->printf("Message received\r\n");
//    uart->printf("Message id: 0x%X \r\n", message.getId());
//    uart->printf("Message length: %d\r\n", message.getDataLength());
//    uart->printf("Message contents: ");

    uint8_t* message_payload = message.getPayload();
//    for (int i = 0; i < message.getDataLength(); i++) {
//        uart->printf("0x%02X ", message_payload[i]);
//    }
//    uart->printf("\r\n\r\n");
}

int main() {
    // Initialize system
    core::platform::init();

    // Get CAN instance with loopback enabled
    io::CAN& can   = io::getCAN<io::Pin::PA_12, io::Pin::PA_11>();
    io::UART& uart = io::getUART<io::Pin::UART_TX, io::Pin::UART_RX>(9600);
    can.addIRQHandler(canIRQHandler, &uart);

    // CAN message that will be sent
    uint8_t payload[] = {0xDE, 0xAD, 0xBE, 0xBE, 0xEF, 0x00, 0x01, 0x02};
    io::CANMessage transmit_BMS(1, 8, &payload[0], true);
    io::CANMessage transmit_GFDB(0xA100100, 8, &payload[0], true);
    io::CANMessage transmit_HIB(0x0D0, 8, &payload[0], true);

    uart.printf("Starting CAN testing\r\n");

    io::CAN::CANStatus result;

    // Attempt to join the CAN network
    result = can.connect();

    if (result != io::CAN::CANStatus::OK) {
        uart.printf("Failed to connect to the CAN network\r\n");
        return 1;
    }

    uint8_t count = 0;
    while (true) {
        transmit_BMS.setPayload(payload);
        uart.printf("sending 0x%x\r\n", payload);
        result = can.transmit(transmit_BMS);
        if (result != io::CAN::CANStatus::OK) {
            uart.printf("Failed to send BMS\r\n");
            return 1;
        }

        if (count % 2 == 0) {
            result = can.transmit(transmit_GFDB);
            if (result != io::CAN::CANStatus::OK) {
                uart.printf("Failed to send GFDB\r\n");
                return 1;
            }
        }

        if (count % 4 == 0) {
            result = can.transmit(transmit_HIB);
            if (result != io::CAN::CANStatus::OK) {
                uart.printf("Failed to send HIB\r\n");
                return 1;
            }
        }

        count++;
        time::wait(500);
    }
    return 0;
}
