#ifndef CUI_USBDEVICECDCACM_HPP
#define CUI_USBDEVICECDCACM_HPP

#include "USBDevice.hpp"
#include "UsbDeviceClass.hpp"
#include "core/rtos/EventFlags.hpp"
#include "core/rtos/Queue.hpp"
#include "tx_port.h"
#include "ux_device_class_cdc_acm.h"

namespace core::rtos {

constexpr uint32_t THREAD_COUNT = 2;

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

constexpr uint32_t READ_WRITE_QUEUE_SIZE = 64;

constexpr uint32_t DEVICE_CONNECTION_MASK = 0x01;

class USBDeviceCDCACM : public UsbDeviceClass {

public:
    struct ReadWriteThreadArgs {
        Queue* messageQueue;
        EventFlags* eventFlags;
        uint32_t messageSize;
    };

    USBDeviceCDCACM(uint8_t readQueueMessageSize, uint8_t writeQueueMessageSize);

    void deviceClassInit() override;

    TXError read(void* destination, uint32_t waitOption);

    TXError write(void* messagePointer, uint32_t waitOption);

    size_t appendThreads(Initializable** destination, size_t capacity) override;

    static void cdcAcmActivate(void *cdc_acm_instance);
    static void cdcAcmDeactivate(void *cdc_acm_instance);
    static void cdcAcmParameterChange(void *cdc_acm_instance);

    static void readThreadEntry(ReadWriteThreadArgs* args);
    static void writeThreadEntry(ReadWriteThreadArgs* args);

private:
    static EventFlags eventFlags;

    Queue readQueue;
    Queue writeQueue;

    ReadWriteThreadArgs readThreadArgs;
    ReadWriteThreadArgs writeThreadArgs;

    Thread<ReadWriteThreadArgs*> readThread;
    Thread<ReadWriteThreadArgs*> writeThread;

    static UX_SLAVE_CLASS_CDC_ACM* cdcAcmInstance;
};

}

#endif // CUI_USBDEVICECDCACM_HPP

