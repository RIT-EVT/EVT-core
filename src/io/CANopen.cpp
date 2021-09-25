#include <EVT/io/CANopen.hpp>
#include <EVT/io/types/CANMessage.hpp>

#include <stdint.h>

// Empty namespace of required global variables
namespace {
    EVT::core::IO::CAN* can;
}

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
