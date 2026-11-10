#include <core/io/platform/f4xx/USB/USBDevicef4xx.hpp>

#include "ux_dcd_stm32.h"

namespace core::rtos {

USBDevicef4xx::USBDevicef4xx(BytePoolBase& pool, UsbDeviceClass& deviceClass, const io::UsbOtg::UsbOtgInitConfig& initConfig, PCD_TypeDef* usbOtgDevice)
    : USBDevice(pool, deviceClass), usbOtg(initConfig, usbOtgDevice) {}

void USBDevicef4xx::deviceThreadEntry() {
    usbOtg.init(); //Usb Otg init

    HAL_PCDEx_SetRxFiFo(&usbOtg.usbInit, 0x200);
    HAL_PCDEx_SetTxFiFo(&usbOtg.usbInit, 0, 0x10);
    HAL_PCDEx_SetTxFiFo(&usbOtg.usbInit, 1, 0x10);
    HAL_PCDEx_SetTxFiFo(&usbOtg.usbInit, 2, 0x20);

    ux_dcd_stm32_initialize((ULONG)usbOtg.usbOtgDevice, (ULONG)&usbOtg.usbInit);

    /* Start the USB device */
    HAL_PCD_Start(&usbOtg.usbInit);
};

}

