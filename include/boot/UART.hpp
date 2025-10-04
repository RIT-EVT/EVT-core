#ifndef EVT_BOOTUART_H
#define EVT_BOOTUART_H

#include <boot/IOInterface.hpp>

class UART: public IOInterface {
public:
    UART();

    void read(uint8_t* data, size_t size) override;

    void write(uint8_t* data, size_t size) override;

    void setIRQHandler(void(* handler)(IOInterface& inter, void* priv), void* priv) override;
};
#endif // EVT_BOOTUART_H
