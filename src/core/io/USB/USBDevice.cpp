#include <core/io/USB/USBDevice.hpp>

#include <core/io/USB/ux_device_descriptors.hpp>
#include <ux_api.h>

namespace core::rtos {

USBDevice::USBDevice(BytePoolBase& pool, UsbDeviceClass& deviceClass)
    : Initializable((char*) "UsbDevice"),
      deviceThread((char*) "Device Thread", deviceThreadEntryTrampoline, this, APPLICATION_THREAD_STACK_SIZE,
                   APPLICATION_THREAD_PRIORITY, APPLICATION_THREAD_PREEMPT_THRESHOLD, APPLICATION_THREAD_TIME_SLICE,
                   APPLICATION_THREAD_AUTOSTART),
      deviceClass(deviceClass) {
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

    deviceClass.deviceClassInit();
}

TXError USBDevice::init(BytePoolBase& pool) {
    Initializable* initializables[MAX_THREADS];

    initializables[0] = &deviceThread;

    size_t count = deviceClass.appendThreads(
        initializables + 1,
        MAX_THREADS
    );

    return bulkInitialize(initializables, count, pool);
};

void USBDevice::deviceThreadEntryTrampoline(USBDevice* device) {
    device->deviceThreadEntry();
}

}

