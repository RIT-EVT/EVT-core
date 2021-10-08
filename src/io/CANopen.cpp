#include <EVT/io/CANopen.hpp>
#include <EVT/io/types/CANMessage.hpp>

#include <stdint.h>

/*
 * Empty namespace to contain "global" variables. These will be used within
 * the driver implementations.
 */
namespace {
    EVT::core::IO::CAN* can;
    EVT::core::DEV::Timer* timer;
    // Temporary "storage" to allow the NVM to work, do not use as actual NVM 
    uint8_t testerStorage[64]; 
}

///////////////////////////////////////////////////////////////////////////////
// Forward declarations of CANopen stack CAN functions
///////////////////////////////////////////////////////////////////////////////
static void canInit(void);
static void canEnable(uint32_t baudrate);
static int16_t canSend(CO_IF_FRM *frm); 
static int16_t canRead(CO_IF_FRM *frm);
static void canReset(void);
static void canClose(void);

///////////////////////////////////////////////////////////////////////////////
// Forward declarations of CANopen stack timer functions
///////////////////////////////////////////////////////////////////////////////
static void timerInit(uint32_t freq);
static void timerReload(uint32_t reload);
static void timerStart(void);
static uint8_t timerUpdate(void); 
static uint32_t timerDelay(void);
static void timerStop(void);

///////////////////////////////////////////////////////////////////////////////
// Forward declarations of CANopen stack NVM functions
///////////////////////////////////////////////////////////////////////////////
static void nvmInit(void);
static uint32_t nvmRead(uint32_t start, uint8_t* buffer, uint32_t size);
static uint32_t nvmWrite(uint32_t start, uint8_t* buffer, uint32_t size);

namespace EVT::core::IO {
    void getCANopenDriver(CAN& canIntf, CO_IF_CAN_DRV* canDriver) {
        can = &canIntf;

        canDriver->Init = canInit;
        canDriver->Enable = canEnable;
        canDriver->Read = canRead;
        canDriver->Send = canSend;
        canDriver->Reset = canReset;
        canDriver->Close = canClose;
    }
}

///////////////////////////////////////////////////////////////////////////////
// Implementation of CANopen stack CAN drivers
///////////////////////////////////////////////////////////////////////////////
/**
 * Initialize the CAN driver. This doesn't do anything since the CAN interface
 * should be passed into `getCANopenDriver` pre-initialized.
 */
static void canInit(void) {

}

/**
 * Enable the CAN driver at the specific baudrate. Again this doesn't
 * do anything since the CAN interface will already be setup.
 */
static void canEnable(uint32_t baudrate) {
    // TODO: Should have the ability to reset the CAN driver
}

/**
 * Send a CAN message. This will convert the CANopen stack CAN message
 * format into the EVT::core::IO::CANMessage
 *
 * @param frm[in] The message to send over cat
 * @return sizeof(CO_IF_FRM) on success, ((int16_t)-1u) on failure
 */
static int16_t canSend(CO_IF_FRM *frm) {
    EVT::core::IO::CANMessage message(frm->Identifier, frm->DLC, frm->Data);
    can->transmit(message);

    return sizeof(CO_IF_FRM);
}

/**
 * Read in a CAN message. This will convert from the EVT::core::IO::CANMessage
 * into the CANopen stack format.
 *
 * @param frm[out] The message to populate with CAN data
 * @return sizeof(CO_IF_FMR) on success, ((int16_t)-1u) on failure
 */
static int16_t canRead(CO_IF_FRM *frm) {
    EVT::core::IO::CANMessage message;

    // No message
    if(!can->receive(&message)) 
        return ((int16_t)-1u);

    frm->Identifier = message.getId();
    frm->DLC = message.getDataLength();
    // Copy contents into payload buffer
    for(int i = 0; i < message.getDataLength(); i++) {
        frm->Data[i] = message.getPayload()[i];
    }

    return sizeof(CO_IF_FRM);
}

/**
 * Reset the CAN interface. This does nothing at the moment.
 */
static void canReset(void) {

}

/**
 * Close the CAN connection. This does nothing at the moment.
 */
static void canClose(void) {

}

///////////////////////////////////////////////////////////////////////////////
// Implementations of CANopen stack timer functions
///////////////////////////////////////////////////////////////////////////////
/**
 * Initiailize the timer driver.
 */
static void timerInit(uint32_t freq) {
    frequency = freq;
}

static void timerReload(uint32_t reload) {
    targetReload = reload;
}

/**
 * Start the "timer" running
 */
static void timerStart(void) {
    startTime = rtc->getTime();
}

/**
 * Return true if the timer has gone off
 */
static uint8_t timerUpdate(void) {
    uint32_t targetTime = startTime + (targetReload / frequency);
    return rtc->getTime() >= targetTime;
}

/**
 * Get the reload value that was set
 */
static uint32_t timerDelay(void) {
    return targetReload;
}

/**
 * Stop the timer, currently does nothing.
 */
static void timerStop(void) {
    
}

///////////////////////////////////////////////////////////////////////////////
// Implementation of CANopen stack NVM functions
///////////////////////////////////////////////////////////////////////////////
/**
 * Initialize the NVM driver, does nothing
 */
static void nvmInit(void) {

}

/**
 * Read from the temporary buffer
 */
static uint32_t nvmRead(uint32_t start, uint8_t* buffer, uint32_t size) {
    uint32_t bytesRead = 0;

    for(unsigned int i = 0; i < size && i + start < MAX_SIZE; i++) {
        buffer[i] = testerStorage[i + start];
        bytesRead++;
    }

    return bytesRead;
}

/**
 * Write to the temporary buffer
 */
static uint32_t nvmWrite(uint32_t start, uint8_t* buffer, uint32_t size) {
    uint32_t bytesWrote = 0;

    for(unsigned int i = 0; i < size && i + start < MAX_SIZE; i++) {
        testerStorage[i + start] = buffer[i];
        bytesWrote++;
    }
    return bytesWrote;
}
