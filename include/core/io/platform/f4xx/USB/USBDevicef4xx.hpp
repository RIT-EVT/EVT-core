#ifndef CUI_USBDEVICEF4XX_HPP
#define CUI_USBDEVICEF4XX_HPP

#include "HALf4/stm32f4xx_hal_pcd.h"
#include "core/io/USB/UsbOtg.hpp"
#include "core/io/USB/USBDevice.hpp"

namespace core::rtos {

class USBDevicef4xx : public USBDevice{
public:
    USBDevicef4xx(BytePoolBase& pool, UsbDeviceClass& deviceClass, const io::UsbOtg::UsbOtgInitConfig& initConfig, PCD_TypeDef* usbOtgDevice);

private:
    void deviceThreadEntry() override;
    io::UsbOtgf4xx usbOtg;
};

}

#endif // CUI_USBDEVICEF4XX_HPP
