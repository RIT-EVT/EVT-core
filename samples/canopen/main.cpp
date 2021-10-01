/**
 * This sample shows off CANopen support from EVT-core. This will
 * setup a CANopen node and attempt to make back and forth communication.
 */
#include <EVT/io/ADC.hpp>
#include <EVT/io/manager.hpp>
#include <EVT/io/UART.hpp>
#include <EVT/io/CAN.hpp>
#include <EVT/dev/platform/f3xx/f302x8/RTC302x8.hpp>
#include <EVT/utils/time.hpp>

#include <EVT/io/CANopen.hpp>
#include <EVT/io/CANopenNVM.hpp>
#include <EVT/dev/CANopenTimer.hpp>

#include <Canopen/co_tmr.h>
#include <Canopen/co_if.h>
#include <Canopen/co_core.h>

namespace IO = EVT::core::IO;
namespace DEV = EVT::core::DEV;
namespace time = EVT::core::time;

///////////////////////////////////////////////////////////////////////////////\
// Data that will be accessible over CAN. This will be application specific
// and ideally not be global variables
///////////////////////////////////////////////////////////////////////////////
uint8_t sampleData = 0;
uint8_t DemoErrReg = 0;
uint8_t DemoString[] = "TEST";    /* string memory */
uint32_t syncId = 0x80;

CO_OBJ_STR DemoStringObj {
    .Offset = 0,
    .Start = &DemoString[0],
};

///////////////////////////////////////////////////////////////////////////////
// CAN object dictionary. This represents the data that is exposed by this
// device on the CAN network
///////////////////////////////////////////////////////////////////////////////
CO_OBJ_T canObjectDictionary[] = {
    { CO_KEY(0x1005, 0, CO_UNSIGNED32|CO_OBJ____R_), 0, (uintptr_t)&syncId },
    { CO_KEY(0x1018, 1, CO_STRING|CO_OBJ____R_), CO_TSTRING, (uintptr_t)&DemoStringObj },
    { CO_KEY(0x1018, 2, CO_STRING|CO_OBJ____R_), CO_TSTRING, (uintptr_t)&DemoStringObj },
    { CO_KEY(0x1018, 3, CO_STRING|CO_OBJ____R_), CO_TSTRING, (uintptr_t)&DemoStringObj },
    { CO_KEY(0x1018, 4, CO_STRING|CO_OBJ____R_), CO_TSTRING, (uintptr_t)&DemoStringObj },



    { CO_KEY(0x1234, 0, CO_UNSIGNED8|CO_OBJ____RW), 0, (uintptr_t)sampleData }
};


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
    
    // Start RTC and set the default time
    time::TimeStamp time;
    time.year = 2021;
    time.month = 9;
    time.day = 10;

    time.hour = 13;
    time.minute = 42;
    time.second = 35;

    DEV::RTCf302x8 rtc;
    rtc.setTime(time);

    // Not sure if this is needed
    uint8_t sdoBuffer[50];

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

    getCANopenDriver(can, &canDriver); 
    getCANopenTimerDriver(rtc, &timerDriver);
    IO::getCANopenNVMDriver(&nvmDriver);

    canStackDriver.Can = &canDriver;
    canStackDriver.Timer = &timerDriver;
    canStackDriver.Nvm = &nvmDriver;
    
    CO_NODE_SPEC canSpec = {
        .NodeId = 0x01,
        .Baudrate = 500000,
        .Dict = &canObjectDictionary[0],
        .DictLen = 5,
        .EmcyCode = NULL,
        .TmrMem = NULL,
        .TmrNum = 0,
        .TmrFreq = 8000000,
        .Drv = &canStackDriver,
        .SdoBuf = &sdoBuffer[0],
    };

    CO_NODE canNode;

    CONodeInit(&canNode, &canSpec);
    CONodeStart(&canNode);

    time::wait(500);

    uart.printf("Error: %d\r\n", CONodeGetErr(&canNode));

    while (1) {
        if(sampleData != 0) {
            uart.printf("Value of my number: %d\n\r", sampleData);
        }
        CONodeProcess(&canNode);
        time::wait(10);
    }
}
