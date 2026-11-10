#ifndef CUI_USB_HPP
#define CUI_USB_HPP

#include <core/rtos/Thread.hpp>
#include <core/rtos/Threadx.hpp>

#include <core/io/USB/UsbDeviceClass.hpp>
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

constexpr uint32_t MAX_THREADS = 8;

namespace core::rtos {

class USBDevice : public Initializable {

public:

    enum class Status {
        OK      = 0x00U,
        ERROR   = 0x01U,
        BUSY    = 0x02U,
        TIMEOUT = 0x03U,
    };

    virtual ~USBDevice() = default;

    USBDevice(BytePoolBase& pool, UsbDeviceClass& deviceClass);

    TXError init(BytePoolBase& pool) override;

private:
    static VOID deviceThreadEntryTrampoline(USBDevice* device);
    virtual void deviceThreadEntry() = 0;
    Thread<USBDevice*> deviceThread;

    UsbDeviceClass& deviceClass;
};

}



#endif // CUI_USB_HPP
