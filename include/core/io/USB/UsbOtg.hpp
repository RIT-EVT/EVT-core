#ifndef CUI_USBOTG_HPP
#define CUI_USBOTG_HPP

namespace core::io {

constexpr uint8_t DEVICE_ENDPOINTS = 8;
constexpr uintptr_t HIGH_SPEED = 0x40040000UL;
constexpr uintptr_t FULL_SPEED = 0x50000000UL;

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

    virtual void init() = 0;

};


}

#endif // CUI_USBOTG_HPP
