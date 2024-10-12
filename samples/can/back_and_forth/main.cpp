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
    uart->printf("Message received\r\n");
    uart->printf("Message id: 0x%X \r\n", message.getId());
    uart->printf("Message length: %d\r\n", message.getDataLength());
    uart->printf("Message contents: ");

    uint8_t* message_payload = message.getPayload();
    for (int i = 0; i < message.getDataLength(); i++) {
        uart->printf("0x%02X ", message_payload[i]);
    }
    uart->printf("\r\n\r\n");
}

int main() {
    // Initialize system
    core::platform::init();

    // Get CAN instance with loopback enabled
    io::CAN& can   = io::getCAN<io::Pin::PA_12, io::Pin::PA_11>();
    io::UART& uart = io::getUART<io::Pin::UART_TX, io::Pin::UART_RX>(9600);
    can.addIRQHandler(canIRQHandler, &uart);

    uint8_t count = 0;
// CAN message that will be sent
#ifdef STM32F446xx
    uint8_t payload[] = {0xDE, 0xAD, 0xBE, 0xBE, 0xEF, 0x00, 0x01, 0x02};
    uint8_t id        = 4;
#else
    uint8_t payload[] = {0xBE, 0xEF, 0xBE, 0xDE, 0xAD, 0x09, 0x08, 0x07};
    uint8_t id        = 3;
#endif

    io::CANMessage transmit_message(id, 8, &payload[0], false);
    io::CANMessage received_message;

    uart.printf("Starting CAN testing\r\n");

    io::CAN::CANStatus result;

    // Attempt to join the CAN network
    result = can.connect();

    if (result != io::CAN::CANStatus::OK) {
        uart.printf("Failed to connect to the CAN network\r\n");
        return 1;
    }

    while (true) {
        // Transmit every second
        payload[7] = count;
        result     = can.transmit(transmit_message);
        if (result != io::CAN::CANStatus::OK) {
            uart.printf("Failed to transmit message\r\n");
            return 1;
        }
        count++;

        time::wait(1000);
    }

    return 0;
}
