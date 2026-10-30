#include <core/io/platform/f4xx/USB/USBDevicef4xx.hpp>

namespace core::rtos {

USBDevicef4xx::USBDevicef4xx(BytePoolBase& pool, uint32_t* readData, uint32_t* writeData, const UsbOtgInitConfig& initConfig, PCD_TypeDef* usbOtgDevice)
    : USBDevice(pool, readData, writeData), UsbOtgf4xx(initConfig, usbOtgDevice) {};

void USBDevicef4xx::deviceThreadEntry() {
    init(); //Usb Otg init

    HAL_PCDEx_SetRxFiFo(&usbInit, 0x200);
    HAL_PCDEx_SetTxFiFo(&usbInit, 0, 0x10);
    HAL_PCDEx_SetTxFiFo(&usbInit, 1, 0x10);
    HAL_PCDEx_SetTxFiFo(&usbInit, 2, 0x20);

    ux_dcd_stm32_initialize((ULONG)usbOtgDevice, (ULONG)&usbInit);

    /* Start the USB device */
    HAL_PCD_Start(&usbInit);
};

}

