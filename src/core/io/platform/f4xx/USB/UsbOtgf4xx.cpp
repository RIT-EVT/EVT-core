#include <core/io/platform/f4xx/USB/UsbOtgf4xx.hpp>

namespace core::io {

UsbOtgf4xx::UsbOtgf4xx(const UsbOtgInitConfig& initConfig, PCD_TypeDef* usbOtgDevice)
    : UsbOtg(initConfig), usbOtgDevice(usbOtgDevice), usbInit() {

    usbInit.Instance = this->usbOtgDevice;
    usbInit.Init.dev_endpoints = initConfig.deviceEndpoints;
    usbInit.Init.speed = initConfig.speed;
    usbInit.Init.dma_enable = initConfig.dmaEnable;
    usbInit.Init.phy_itface = initConfig.physicalInterface;
    usbInit.Init.Sof_enable = initConfig.startOfFrameEnable;
    usbInit.Init.lpm_enable = initConfig.lowPowerEnable;
    usbInit.Init.lpm_enable = initConfig.linkPowerModeEnable;
    usbInit.Init.vbus_sensing_enable = initConfig.vbusSensingEnable;
    usbInit.Init.use_dedicated_ep1 = initConfig.useDedicatedEp1;
    usbInit.Init.use_external_vbus = initConfig.useExternalVbus;
}

void UsbOtgf4xx::init() {
    HAL_PCD_Init(&usbInit);
}

} // namespace core::io