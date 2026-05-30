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

    void write(uint8_t* data, size_t size);

    void read(uint8_t* data, size_t size);

    void setIRQHandler(void (*handler)(IOInterface& inter, void* priv), void* priv);

    /**
     * Print a formatted string over UART.
     *
     * @param format The string format to be printed out.
     */
    virtual void printf(const char* format, ...);

private:
    uint32_t baudrate = 115200;

protected:
    /** Function pointer to call for the interrupt handler */
    void (*handler)(IOInterface& inter, void* priv);
    /** Context to pass into the IRQ handler */
    void* priv;
};
} // namespace boot
#endif // EVT_BOOT_UART_HPP
