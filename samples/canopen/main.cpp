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
uint8_t sampleData;

///////////////////////////////////////////////////////////////////////////////
// CAN object dictionary. This represents the data that is exposed by this
// device on the CAN network
///////////////////////////////////////////////////////////////////////////////
CO_OBJ_T canObjectDictionary[] = {
    { CO_KEY(0x1001, 0, CO_UNSIGNED8|CO_OBJ____RW), 0, (uintptr_t)sampleData }
};

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
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);
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

    ///////////////////////////////////////////////////////////////////////////
    // Setup CAN configuration, this handles making drivers, applying settings.
    // And generally creating the CANopen stack node which is the interface
    // between the application (the code we write) and the physical CAN network
    ///////////////////////////////////////////////////////////////////////////
    // Make drivers
    CO_IF_DRV canStackDriver;
    getCANopenDriver(can, (CO_IF_CAN_DRV*)(canStackDriver.Can)); 
    getCANopenTimerDriver(rtc, (CO_IF_TIMER_DRV*)canStackDriver.Timer);
    IO::getCANopenNVMDriver((CO_IF_NVM_DRV*)canStackDriver.Nvm);
    
    CO_NODE_SPEC canSpec = {
        .NodeId = 0x01,
        .Baudrate = 500000,
        .Dict = &canObjectDictionary[0],
        .DictLen = 1,
        .EmcyCode = NULL,
        .TmrMem = NULL,
        .TmrNum = 0,
        .TmrFreq = 8000000,
        .Drv = &canStackDriver,
        .SdoBuf = NULL
    };

    CO_NODE canNode;

    CONodeInit(&canNode, &canSpec);

    time::wait(500);

    IO::ADC& adc0 = IO::getADC<IO::Pin::PA_0>();
    IO::ADC& adc1 = IO::getADC<IO::Pin::PA_1>();

    while (1) {
        uart.printf("--------------------\r\n");
        uart.printf("ADC0 : %.2fV\r\n", adc0.read());
        uart.printf("ADC0: %.2f%%\r\n", adc0.readPercentage() * 100);
        uart.printf("ADC0 raw: %d\r\n\r\n", adc0.readRaw());

        uart.printf("ADC1: %.2fV\r\n", adc1.read());
        uart.printf("ADC1: %.2f%%\r\n", adc1.readPercentage());
        uart.printf("ADC1 raw: %d\r\n", adc1.readRaw());

        uart.printf("--------------------\r\n\r\n");
        time::wait(500);
    }
}
