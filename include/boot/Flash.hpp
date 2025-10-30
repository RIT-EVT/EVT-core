#ifndef EVT_BOOT_FLASH_HPP
#define EVT_BOOT_FLASH_HPP

#include <boot/IOInterface.hpp>

namespace boot {

enum class FlashStatus {
    SUCCESS         = 0,
    FAILURE         = 1,
    FLASH_BUSY      = 2,
    RESOURCE_LOCKED = 3,
    ACCESS_FAILURE  = 4,
    FAILED_WRITE    = 5,
    FAILED_READ     = 6,
};

/**
 * This is the abstract class for Flash memory.
 */
class Flash : public IOInterface {
public:
    /**
     * Constructor for the bootloader Flash memory class.
     */
    Flash();

    /**
     * Sets the flash memory to read only.
     *
     * @return An error code based on if the memory was succesfully locked. Zero if succesfully locked.
     */
    virtual FlashStatus lock();

    /**
     * Sets the flash memory to read and write.
     *
     * @return An error code based on if the memory was successfully locked. Zero if successfully locked.
     */
    virtual FlashStatus unlock();

    /**
     * Erases pages of memory.
     *
     * @param pageNum The page number to be erased.
     * @return An error code based on if the memory was successfully erased. Zero if successfully erased.
     */
    virtual FlashStatus erasePage(int pageNum);
};
} // namespace boot

#endif // EVT_BOOT_FLASH_HPP
