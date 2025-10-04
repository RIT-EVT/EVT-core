#ifndef EVT_IOINTERFACE_HPP
#define EVT_IOINTERFACE_HPP

#include <cstdint>

class IOInterface {
public:
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
     *
     * @param handler The interrupt handler.
     * @param priv Private data.
     */
    virtual void setIRQHandler(void (*handler)(IOInterface& inter, void* priv), void* priv);
};

#endif // EVT_IOINTERFACE_HPP
