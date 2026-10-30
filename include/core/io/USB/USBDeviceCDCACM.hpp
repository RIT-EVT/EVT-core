#ifndef CUI_USBDEVICECDCACM_HPP
#define CUI_USBDEVICECDCACM_HPP

#include "USBDevice.hpp"
#include "core/rtos/EventFlags.hpp"
#include "tx_port.h"

namespace core::rtos {

class USBDeviceCDCACM {

public:
    typedef struct {
        EventFlags* flags;
        uint32_t* data;
    } readWriteThreadArgs;

    static void readThreadEntry(readWriteThreadArgs* args);
    static void writeThreadEntry(readWriteThreadArgs* args);

    static void cdcAcmActivate(void *cdc_acm_instance);
    static void cdcAcmDeactivate(void *cdc_acm_instance);
    static void cdcAcmParameterChange(void *cdc_acm_instance);
};

}

#endif // CUI_USBDEVICECDCACM_HPP

