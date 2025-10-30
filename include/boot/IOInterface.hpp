#ifndef EVT_BOOT_IOINTERFACE_HPP
#define EVT_BOOT_IOINTERFACE_HPP

#include <cstdint>

namespace boot {

/**
 * Abstract class for the bootloaders interfaces to read and write the data from the peripherals. The point of this
 * class is to reimplement the peripherals without having to use all of EVT-Core, allowing for less memory usage.
 */
class IOInterface {
public:
    /**
     * Constructor for the interface.
     */
    IOInterface();

    /**
     * Read an arbitrary number of bytes out.
     *
     * @param[in] data The data to send out.
     * @param[in] size The number of bytes to send out over UART.
     */
    virtual void read(uint8_t* data, size_t size) = 0;

    /**
     * Write an arbitrary number of bytes out.
     *
     * @param[in] data The data to send out.
     * @param[in] size The number of bytes to send out.
     */
    virtual void write(uint8_t* data, size_t size) = 0;

    /**
     * Sets the handler for IRQ and assign it a private data value.
     *
     * @param handler The interrupt handler.
     * @param priv Private data.
     */
    virtual void setIRQHandler(void (*handler)(IOInterface& inter, void* priv), void* priv);
};
} // namespace boot

#endif // EVT_BOOT_IOINTERFACE_HPP
