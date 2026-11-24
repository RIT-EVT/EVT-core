#include <core/io/USB/UsbDeviceClass.hpp>

#include <core/io/USB/USBDevice.hpp>

namespace core::rtos::usb {

UsbDeviceClass::UsbDeviceClass(USBDevice& device) : Initializable((char*) "USB Device Class"), device(device) {}

} // namespace core::rtos