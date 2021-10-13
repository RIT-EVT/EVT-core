/**
 * This sample shows off CANopen support from EVT-core. This will
 * setup a CANopen node and attempt to make back and forth communication.
 */
#include <stdint.h>

#include <EVT/io/ADC.hpp>
#include <EVT/io/manager.hpp>
#include <EVT/io/UART.hpp>
#include <EVT/io/CAN.hpp>
#include <EVT/utils/time.hpp>
#include <EVT/dev/platform/f3xx/f302x8/Timerf302x8.hpp>

#include <EVT/io/CANopen.hpp>

#include <Canopen/co_tmr.h>
#include <Canopen/co_if.h>
#include <Canopen/co_core.h>

#include "TestCanNode.hpp"

namespace IO = EVT::core::IO;
namespace DEV = EVT::core::DEV;
namespace time = EVT::core::time;

IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);

///////////////////////////////////////////////////////////////////////////////
// Callbacks
///////////////////////////////////////////////////////////////////////////////
extern "C" void CONodeFatalError(void) {

}

extern "C" void COIfCanReceive(CO_IF_FRM *frm) {

}

extern "C" int16_t COLssStore(uint32_t baudrate, uint8_t nodeId) {
    return 0;
}

extern "C" int16_t COLssLoad(uint32_t *baudrate, uint8_t *nodeId) {
    return 0;
}

extern "C" void CONmtModeChange(CO_NMT *nmt, CO_MODE mode) {
    uart.printf("mode is now: %d\r\n", mode);
}

extern "C" void CONmtHbConsEvent(CO_NMT *nmt, uint8_t nodeId) {

}

extern "C" void CONmtHbConsChange(CO_NMT *nmt, uint8_t nodeId, CO_MODE mode) {

}

extern "C" int16_t COParaDefault(CO_PARA *pg) {
    return 0;
}

extern "C" void COPdoTransmit(CO_IF_FRM *frm) {

}

extern "C" int16_t COPdoReceive(CO_IF_FRM *frm) {
    return 0;
}

extern "C" void COPdoSyncUpdate(CO_RPDO *pdo) {

}

extern "C" void COTmrLock(void) {

}

extern "C" void COTmrUnlock(void) {

}

int main() {
    // Initialize system
    IO::init();

    // Intialize peripherals
    IO::CAN& can = IO::getCAN<IO::Pin::PA_12, IO::Pin::PA_11>();
    DEV::Timerf302x8 timer(TIM2, 100, nullptr);
    timer.stopTimer();

    TestCanNode testCanNode;

    // Reserved memory for CANopen stack usage
    uint8_t sdoBuffer[1][CO_SDO_BUF_BYTE];
    CO_TMR_MEM appTmrMem[4];

    ///////////////////////////////////////////////////////////////////////////
    // Setup CAN configuration, this handles making drivers, applying settings.
    // And generally creating the CANopen stack node which is the interface
    // between the application (the code we write) and the physical CAN network
    ///////////////////////////////////////////////////////////////////////////
    // Make drivers
    CO_IF_DRV canStackDriver;

    CO_IF_CAN_DRV canDriver;
    CO_IF_TIMER_DRV timerDriver;
    CO_IF_NVM_DRV nvmDriver;

    IO::getCANopenCANDriver(can, &canDriver);
    IO::getCANopenTimerDriver(timer, &timerDriver);
    IO::getCANopenNVMDriver(&nvmDriver);

    canStackDriver.Can = &canDriver;
    canStackDriver.Timer = &timerDriver;
    canStackDriver.Nvm = &nvmDriver;

    CO_NODE_SPEC canSpec = {
        .NodeId = 0x01,
        .Baudrate = 500000,
        .Dict = testCanNode.getObjectDictionary(),
        .DictLen = testCanNode.getNumElements(),
        .EmcyCode = NULL,
        .TmrMem = appTmrMem,
        .TmrNum = 16,
        .TmrFreq = 100,
        .Drv = &canStackDriver,
        .SdoBuf = (uint8_t *)&sdoBuffer[0],
    };

    CO_NODE canNode;

    CONodeInit(&canNode, &canSpec);
    CONodeStart(&canNode);
    CONmtSetMode(&canNode.Nmt, CO_OPERATIONAL);

    time::wait(500);

    uart.printf("Error: %d\r\n", CONodeGetErr(&canNode));

    while (1) {
        // Trigger PDO messages
        // COTPdoTrigPdo(&canNode.TPdo[0], 0);
        uart.printf("Value of my number: %d\n\r", testCanNode.getSampleData());
        CONodeProcess(&canNode);
        COTmrService(&canNode.Tmr);
        COTmrProcess(&canNode.Tmr);
        time::wait(10);
    }
}
