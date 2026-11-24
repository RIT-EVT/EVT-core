#ifndef CUI_USB_HPP
#define CUI_USB_HPP

#include "Enums.hpp"

#include <core/rtos/Thread.hpp>
#include <core/rtos/Threadx.hpp>

#include <core/rtos/BytePoolBase.hpp>
#include <ux_device_class_cdc_acm.h>

constexpr uint32_t DEVICE_MEMORY_STACK_SIZE = 6 * 1024;

constexpr uint32_t APPLICATION_THREAD_STACK_SIZE = 1024;
constexpr uint32_t APPLICATION_THREAD_PRIORITY = 10;
constexpr uint32_t APPLICATION_THREAD_PREEMPT_THRESHOLD = 10;
constexpr uint32_t APPLICATION_THREAD_TIME_SLICE = TX_NO_TIME_SLICE;
constexpr uint32_t APPLICATION_THREAD_AUTOSTART = true;

namespace core::rtos::usb {

class USBDevice : public Initializable {

public:

    enum class Status {
        OK      = 0x00U,
        ERROR   = 0x01U,
        BUSY    = 0x02U,
        TIMEOUT = 0x03U,
    };

    virtual ~USBDevice() = default;

    USBDevice();

    TXError init(BytePoolBase& pool) override;

    Thread<USBDevice*>* getDeviceThread();

private:
    static VOID deviceThreadEntryTrampoline(USBDevice* device);
    virtual void deviceThreadEntry() = 0;
    Thread<USBDevice*> deviceThread;
};

}



#endif // CUI_USB_HPP
