//
// Created by oscarc on 10/17/26.
//

#ifndef CUI_USBOTG_HPP
#define CUI_USBOTG_HPP

#include <cstdint>

namespace core::io {

constexpr uint8_t DEVICE_ENDPOINTS = 8;

class UsbOtg {

public:
    struct UsbOtgInitConfig {
        uint8_t deviceEndpoints;
        uint8_t speed;
        uint8_t dmaEnable;
        uint8_t physicalInterface;
        uint8_t startOfFrameEnable;
        uint8_t lowPowerEnable;
        uint8_t linkPowerModeEnable;
        uint8_t vbusSensingEnable;
        uint8_t useDedicatedEp1;
        uint8_t useExternalVbus;
    };

    virtual ~UsbOtg() = default;

    UsbOtg(const UsbOtgInitConfig& initConfig);

    virtual void init() = 0;

private:
    UsbOtgInitConfig initConfig;
};


}

#endif // CUI_USBOTG_HPP
