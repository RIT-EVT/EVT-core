/**
 * Listens to a CANopen bus and prints every PDO it sees, decoded into
 * Node-ID / TPDO-NUM / value instead of the raw COB-ID.
 *
 * @author Collin Bolles
 */
#include <core/io/CAN.hpp>
#include <core/manager.hpp>
#include <core/utils/time.hpp>

namespace io   = core::io;
namespace time = core::time;

// CANopen predefined connection set: COB-ID = (function code << 7) | Node-ID
constexpr uint32_t CANOPEN_NODE_ID_MASK = 0x7F;
constexpr uint32_t CANOPEN_FUNC_MASK    = 0x780;

constexpr uint32_t TPDO_BASE  = 0x180;
constexpr uint32_t TPDO_INC   = 0x100;
constexpr uint8_t  TPDO_COUNT = 4;

void canIRQHandler(io::CANMessage& message, void* priv) {
    io::UART* uart = (io::UART*) priv;

    uint32_t id     = message.getId();
    uint32_t nodeId = id & CANOPEN_NODE_ID_MASK;
    uint32_t base   = id & CANOPEN_FUNC_MASK;

    int8_t tpdoNum = -1;
    for (uint8_t i = 0; i < TPDO_COUNT; i++) {
        if (base == TPDO_BASE + (i * TPDO_INC)) {
            tpdoNum = (int8_t) i;
            break;
        }
    }

    if (tpdoNum < 0) {
        uart->printf("Node-ID: %d\tNon-TPDO id: 0x%X\r\n", nodeId, id);
        return;
    }

    uint8_t* payload = message.getPayload();
    uint16_t value    = (uint16_t) (payload[0] | (payload[1] << 8));

    uart->printf("Node-ID: %d\tTPDO: %d\tValue: %u\r\n", nodeId, tpdoNum, value);
}

int main() {
    // Initialize system
    core::platform::init();

    io::CAN& can   = io::getCAN<io::Pin::PA_12, io::Pin::PA_11>();
    io::UART& uart = io::getUART<io::Pin::UART_TX, io::Pin::UART_RX>(9600);
    can.addIRQHandler(canIRQHandler, &uart);

    uart.printf("Starting CANopen sniffing\r\n");

    io::CAN::CANStatus result = can.connect();

    if (result != io::CAN::CANStatus::OK) {
        uart.printf("Failed to connect to the CAN network\r\n");
        return 1;
    }

    while (true) {
        time::wait(1000);
    }

    return 0;
}
