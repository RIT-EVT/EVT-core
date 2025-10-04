#ifndef EVT_BOOTCAN_HPP
#define EVT_BOOTCAN_HPP

#include <boot/IOInterface.hpp>

class CAN : public IOInterface {
public:
    CAN();

    void read(uint8_t* data, size_t size) override;

    void write(uint8_t* data, size_t size) override;

    void setIRQHandler(void(* handler)(IOInterface& inter, void* priv), void* priv) override;
};

#endif // EVT_BOOTCAN_HPP
