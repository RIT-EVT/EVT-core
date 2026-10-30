//
// Created by oscarc on 10/29/26.
//

#ifndef CUI_USBDEVICEF4XX_HPP
#define CUI_USBDEVICEF4XX_HPP

#include "HALf4/stm32f4xx_hal_pcd.h"
#include "UsbOtgf4xx.hpp"
#include "core/io/USB/USBDevice.hpp"
#include <ux_dcd_stm32.h>

namespace core::rtos {

class USBDevicef4xx : public USBDevice, public io::UsbOtgf4xx {
public:
    USBDevicef4xx(BytePoolBase& pool, uint32_t* readData, uint32_t* writeData, const UsbOtgInitConfig& initConfig, PCD_TypeDef* usbOtgDevice);

private:
    void deviceThreadEntry() override;
};

}

#endif // CUI_USBDEVICEF4XX_HPP
