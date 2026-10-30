#ifndef CUI_USBOTGF4XX_HPP
#define CUI_USBOTGF4XX_HPP

#include <HALf4/stm32f4xx_hal.h>
#include <core/io/USB/UsbOtg.hpp>

namespace core::io {

class UsbOtgf4xx : public UsbOtg {

public:
    UsbOtgf4xx(const UsbOtgInitConfig& initConfig, PCD_TypeDef* usbOtgDevice);

    void init() override;
protected:
    PCD_TypeDef* usbOtgDevice;

    PCD_HandleTypeDef usbInit;
};

}

#endif // CUI_USBOTGF4XX_HPP
