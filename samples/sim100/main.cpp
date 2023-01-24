/**
 * This example is to test sending CAN messages to the SIM100
 * GPIO.
 */
#include <EVT/dev/LED.hpp>
#include <EVT/io/GPIO.hpp>
#include <EVT/io/manager.hpp>
#include <EVT/io/pin.hpp>
#include <EVT/utils/time.hpp>

namespace IO = EVT::core::IO;
namespace DEV = EVT::core::DEV;
namespace time = EVT::core::time;

int main() {
    // Initialize system
    IO::init();

    // Get CAN instance with loopback enabled
    IO::CAN& can = IO::getCAN<IO::Pin::PA_12, IO::Pin::PA_11>(true);
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);

    uint8_t payload = 0xE4;

    IO::CANMessage transmit_message(0xA100101, 1, &payload, false);
    IO::CANMessage received_message;

    IO::CAN::CANStatus result;
    uart.printf("Attempting to connect CAN network\r\n");

    // Attempt to join the CAN network
    result = can.connect();

    can.addCANFilter(0, 0, 13);
    can.enableEmergencyFilter(ENABLE);

    uart.printf("Starting CAN testing\r\n");

    if (result != IO::CAN::CANStatus::OK) {
        uart.printf("Failed to connect to CAN network\r\n");
        return 1;
    }

    while (true) {
        result = can.transmit(transmit_message);
        if (result != IO::CAN::CANStatus::OK) {
            uart.printf("Failed to transmit message\r\n");
            return 1;
        }

        result = can.receive(&received_message, false);
        if (result != IO::CAN::CANStatus::OK) {
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
