#ifndef CUI_USBDEVICECLASS_HPP
#define CUI_USBDEVICECLASS_HPP

#include <core/io/USB/USBDevice.hpp>

namespace core::rtos::usb {

class UsbDeviceClass : public Initializable {
public:
    virtual ~UsbDeviceClass()  = default;

    UsbDeviceClass(USBDevice& device);

private:
    USBDevice& device;
};

}

#endif // CUI_USBDEVICECLASS_HPP