#ifndef EVT_BOOT_CAN_HPP
#define EVT_BOOT_CAN_HPP

#include <boot/IOInterface.hpp>

namespace boot {

/**
 * Minimal abstract class for CAN IO Interface in the bootloader. This is different from the core CAN.hpp as it allows
 * the use of CAN without using all of EVT-Core.
 */
class CAN : public IOInterface {
public:
    /**
     * Constructor for the bootloader CAN class.
     */
    CAN();
};
} // namespace boot

#endif // EVT_BOOT_CAN_HPP
