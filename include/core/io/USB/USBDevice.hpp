#ifndef CUI_USB_HPP
#define CUI_USB_HPP

#include <core/rtos/Thread.hpp>
#include <core/rtos/Threadx.hpp>

#include <core/io/USB/USBDeviceCDCACM.hpp>
#include <core/rtos/BytePoolBase.hpp>
#include <core/rtos/EventFlags.hpp>
#include <ux_device_class_cdc_acm.h>

#include <cstdint>

constexpr uint32_t DEVICE_MEMORY_STACK_SIZE = 6 * 1024;

constexpr uint32_t APPLICATION_THREAD_STACK_SIZE = 1024;
constexpr uint32_t APPLICATION_THREAD_PRIORITY = 10;
constexpr uint32_t APPLICATION_THREAD_PREEMPT_THRESHOLD = 10;
constexpr uint32_t APPLICATION_THREAD_TIME_SLICE = TX_NO_TIME_SLICE;
constexpr uint32_t APPLICATION_THREAD_AUTOSTART = true;

constexpr uint32_t READ_THREAD_STACK_SIZE = 1024;
constexpr uint32_t READ_THREAD_PRIORITY = 20;
constexpr uint32_t READ_THREAD_PREEMPT_THRESHOLD = 20;
constexpr uint32_t READ_THREAD_TIME_SLICE = TX_NO_TIME_SLICE;
constexpr uint32_t READ_THREAD_AUTOSTART = true;

constexpr uint32_t WRITE_THREAD_STACK_SIZE = 1024;
constexpr uint32_t WRITE_THREAD_PRIORITY = 20;
constexpr uint32_t WRITE_THREAD_PREEMPT_THRESHOLD = 20;
constexpr uint32_t WRITE_THREAD_TIME_SLICE = TX_NO_TIME_SLICE;
constexpr uint32_t WRITE_THREAD_AUTOSTART = true;

namespace core::rtos {

class USBDevice {

public:

    enum class Status {
        OK      = 0x00U,
        ERROR   = 0x01U,
        BUSY    = 0x02U,
        TIMEOUT = 0x03U,
    };

    virtual ~USBDevice() = default;

    USBDevice(BytePoolBase& pool, uint32_t* readData, uint32_t* writeData);

private:
    EventFlags sharedFlags;

    static VOID deviceThreadEntryTrampoline(USBDevice* device);
    virtual void deviceThreadEntry() = 0;
    Thread<USBDevice*> deviceThread;

    USBDeviceCDCACM::readWriteThreadArgs readThreadArgs;
    Thread<USBDeviceCDCACM::readWriteThreadArgs*> readThread;

    USBDeviceCDCACM::readWriteThreadArgs writeThreadArgs;
    Thread<USBDeviceCDCACM::readWriteThreadArgs*> writeThread;

    ULONG cdcAcmInterfaceNumber;
    ULONG cdcAcmConfigurationNumber;
    UX_SLAVE_CLASS_CDC_ACM_PARAMETER cdcAcmParameter;
};

}



#endif // CUI_USB_HPP
