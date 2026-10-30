#include <core/io/USB/USBDevice.hpp>

#include "core/io/USB/UsbOtg.hpp"
#include "core/manager.hpp"

#include <core/io/USB/ux_device_descriptors.hpp>
#include <ux_api.h>

namespace core::rtos {

USBDevice::USBDevice(BytePoolBase& pool, uint32_t* readData, uint32_t* writeData) :
    sharedFlags((char*) "Shared Flags"),
    deviceThread((char*) "Device Thread",
        deviceThreadEntryTrampoline,
        this,
        APPLICATION_THREAD_STACK_SIZE,
        APPLICATION_THREAD_PRIORITY,
        APPLICATION_THREAD_PREEMPT_THRESHOLD,
        APPLICATION_THREAD_TIME_SLICE,
        APPLICATION_THREAD_AUTOSTART),
    readThreadArgs({&sharedFlags, readData}),
    readThread((char*) "Read Thread",
        USBDeviceCDCACM::readThreadEntry,
        &readThreadArgs,
        READ_THREAD_STACK_SIZE,
        READ_THREAD_PRIORITY,
        READ_THREAD_PREEMPT_THRESHOLD,
        READ_THREAD_TIME_SLICE,
        READ_THREAD_AUTOSTART),
    writeThreadArgs({&sharedFlags, writeData}),
    writeThread((char*) "Write Thread",
        USBDeviceCDCACM::writeThreadEntry,
        &writeThreadArgs,
        WRITE_THREAD_STACK_SIZE,
        WRITE_THREAD_PRIORITY,
        WRITE_THREAD_PREEMPT_THRESHOLD,
        WRITE_THREAD_TIME_SLICE,
        WRITE_THREAD_AUTOSTART)
{
    void* usbMemoryBlockPointer;

    pool.allocateMemory(DEVICE_MEMORY_STACK_SIZE, TXW_NO_WAIT, &usbMemoryBlockPointer);

    ux_system_initialize(usbMemoryBlockPointer, DEVICE_MEMORY_STACK_SIZE, UX_NULL, 0);

    ULONG device_framework_hs_length;
    ULONG device_framework_fs_length;
    ULONG string_framework_length;
    ULONG language_id_framework_length;

    uint8_t* device_framework_high_speed =
        USBD_Get_Device_Framework_Speed(USBD_HIGH_SPEED, &device_framework_hs_length);

    uint8_t* device_framework_full_speed =
        USBD_Get_Device_Framework_Speed(USBD_FULL_SPEED, &device_framework_fs_length);

    uint8_t* string_framework = USBD_Get_String_Framework(&string_framework_length);

    uint8_t* language_id_framework = USBD_Get_Language_Id_Framework(&language_id_framework_length);

    ux_device_stack_initialize(device_framework_high_speed,
                               device_framework_hs_length,
                               device_framework_full_speed,
                               device_framework_fs_length,
                               string_framework,
                               string_framework_length,
                               language_id_framework,
                               language_id_framework_length,
                               nullptr);

    cdcAcmParameter.ux_slave_class_cdc_acm_instance_activate   = USBDeviceCDCACM::cdcAcmActivate;
    cdcAcmParameter.ux_slave_class_cdc_acm_instance_deactivate = USBDeviceCDCACM::cdcAcmDeactivate;
    cdcAcmParameter.ux_slave_class_cdc_acm_parameter_change    = USBDeviceCDCACM::cdcAcmParameterChange;

    cdcAcmConfigurationNumber = USBD_Get_Configuration_Number(CLASS_TYPE_CDC_ACM, 0);
    cdcAcmInterfaceNumber     = USBD_Get_Interface_Number(CLASS_TYPE_CDC_ACM, 0);

    ux_device_stack_class_register(_ux_system_slave_class_cdc_acm_name,
                                   ux_device_class_cdc_acm_entry,
                                   cdcAcmConfigurationNumber,
                                   cdcAcmInterfaceNumber,
                                   &cdcAcmParameter);
}

void USBDevice::deviceThreadEntryTrampoline(USBDevice* device) {
    device->deviceThreadEntry();
}

}

