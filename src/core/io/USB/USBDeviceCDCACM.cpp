#include <core/io/USB/USBDeviceCDCACM.hpp>

#include "core/io/USB/ux_device_descriptors.hpp"
#include "core/io/types/CANMessage.hpp"
#include "ux_api.h"

namespace core::rtos {

EventFlags USBDeviceCDCACM::eventFlags = EventFlags((char *) "CDC ACM Connection Flag");

USBDeviceCDCACM::USBDeviceCDCACM(uint8_t readQueueMessageSize, uint8_t writeQueueMessageSize) :
readQueue((char*) "USB Read Queue", readQueueMessageSize, READ_WRITE_QUEUE_SIZE),
writeQueue((char*) "UQueueSB Write Queue", writeQueueMessageSize, READ_WRITE_QUEUE_SIZE),
readThreadArgs({.messageQueue = &readQueue, .eventFlags = &eventFlags, .messageSize = readQueueMessageSize}),
writeThreadArgs({.messageQueue = &readQueue, .eventFlags = &eventFlags, .messageSize = writeQueueMessageSize}),
readThread((char*) "Read Thread",
    readThreadEntry,
    &readThreadArgs,
    READ_THREAD_STACK_SIZE,
    READ_THREAD_PRIORITY,
    READ_THREAD_PREEMPT_THRESHOLD,
    READ_THREAD_TIME_SLICE,
    READ_THREAD_AUTOSTART),
writeThread((char*) "Write Thread",
    writeThreadEntry,
    &writeThreadArgs,
    WRITE_THREAD_STACK_SIZE,
    WRITE_THREAD_PRIORITY,
    WRITE_THREAD_PREEMPT_THRESHOLD,
    WRITE_THREAD_TIME_SLICE,
    WRITE_THREAD_AUTOSTART)
{}

void USBDeviceCDCACM::deviceClassInit() {
    UX_SLAVE_CLASS_CDC_ACM_PARAMETER cdcAcmParameter;

    cdcAcmParameter.ux_slave_class_cdc_acm_instance_activate   = cdcAcmActivate;
    cdcAcmParameter.ux_slave_class_cdc_acm_instance_deactivate = cdcAcmDeactivate;
    cdcAcmParameter.ux_slave_class_cdc_acm_parameter_change    = cdcAcmParameterChange;

    ULONG cdcAcmConfigurationNumber = USBD_Get_Configuration_Number(CLASS_TYPE_CDC_ACM, 0);
    ULONG cdcAcmInterfaceNumber     = USBD_Get_Interface_Number(CLASS_TYPE_CDC_ACM, 0);

    ux_device_stack_class_register(_ux_system_slave_class_cdc_acm_name,
                                   ux_device_class_cdc_acm_entry,
                                   cdcAcmConfigurationNumber,
                                   cdcAcmInterfaceNumber,
                                   &cdcAcmParameter);

}

TXError USBDeviceCDCACM::read(void* destination, uint32_t waitOption) {
    return readQueue.receive(destination, waitOption);
}

TXError USBDeviceCDCACM::write(void* destination, uint32_t waitOption) {
    return writeQueue.send(destination, waitOption);
}

size_t USBDeviceCDCACM::appendThreads(Initializable** destination, size_t capacity) {
    if (capacity < THREAD_COUNT) {
        return 0;
    }

    destination[0] = &readThread;
    destination[1] = &writeThread;

    return THREAD_COUNT;
}

void USBDeviceCDCACM::cdcAcmActivate(void* cdc_acm_instance) {
    cdcAcmInstance = static_cast<UX_SLAVE_CLASS_CDC_ACM*> (cdc_acm_instance);
    eventFlags.set(DEVICE_CONNECTION_MASK);
}

void USBDeviceCDCACM::cdcAcmDeactivate(void* cdc_acm_instance) {
    cdcAcmInstance = nullptr;
    eventFlags.clear(DEVICE_CONNECTION_MASK);
}

void USBDeviceCDCACM::cdcAcmParameterChange(void* cdc_acm_instance) {
    cdcAcmInstance = static_cast<UX_SLAVE_CLASS_CDC_ACM*> (cdc_acm_instance);
    ULONG lineState;

    UINT status =
        ux_device_class_cdc_acm_ioctl(cdcAcmInstance, UX_SLAVE_CLASS_CDC_ACM_IOCTL_GET_LINE_STATE, &lineState);

    if (status & UX_SLAVE_CLASS_CDC_ACM_LINE_STATE_DTR) { //port is opened
        eventFlags.set(DEVICE_CONNECTION_MASK);
    } else {
        eventFlags.clear(DEVICE_CONNECTION_MASK);
    }
}

void USBDeviceCDCACM::readThreadEntry(ReadWriteThreadArgs* args) {
    uint32_t flagOutput;
    ULONG actualDataLength;
    UCHAR message;

    while (true) {
        if (eventFlags.get(DEVICE_CONNECTION_MASK, false, false, TXW_WAIT_FOREVER, &flagOutput)) {
            ux_device_class_cdc_acm_read(cdcAcmInstance, &message, args->messageSize ,&actualDataLength);
            args->messageQueue->send(&message, TXW_WAIT_FOREVER);
        }
    }
}

void USBDeviceCDCACM::writeThreadEntry(ReadWriteThreadArgs* args) {
    uint32_t flagOutput;
    ULONG actualDataLength;
    UCHAR message;

    while (true) {
        if (eventFlags.get(DEVICE_CONNECTION_MASK, false, false, TXW_WAIT_FOREVER, &flagOutput)) {
            args->messageQueue->receive(&message, TXW_WAIT_FOREVER);
            ux_device_class_cdc_acm_write(cdcAcmInstance, &message, args->messageSize ,&actualDataLength);
        }
    }
}

}