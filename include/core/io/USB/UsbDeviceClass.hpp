#ifndef CUI_USBDEVICECLASS_HPP
#define CUI_USBDEVICECLASS_HPP

namespace core::rtos {

class UsbDeviceClass {
public:
    virtual ~UsbDeviceClass()  = default;

    virtual void deviceClassInit() = 0;

    virtual size_t appendThreads(Initializable** destination, size_t capacity) = 0;

};

}

#endif // CUI_USBDEVICECLASS_HPP