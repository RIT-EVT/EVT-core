#ifndef EVT_BOOT_UART_HPP
#define EVT_BOOT_UART_HPP

#include <boot/IOInterface.hpp>
#include <core/io/pin.hpp>

namespace boot {

/**
 * Minimal abstract class for using UART without having to access EVT-Core.
 */
class UART : public IOInterface {
public:
    /**
     * Constructor for the bootloader UART class.
     */
    UART();

    /**
     * Print a formatted string over UART.
     *
     * @param format The string format to be printed out.
     */
    virtual void printf(const char* format, ...);
};
} // namespace boot
#endif // EVT_BOOT_UART_HPP
