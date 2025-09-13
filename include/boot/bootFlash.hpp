#ifndef EVT_BOOTFLASH_HPP
#define EVT_BOOTFLASH_HPP

namespace boot {

enum class FlashStatus {
    SUCCESS = 0,
    FAILURE = 1,
    FLASH_BUSY = 2,
    RESOURCE_LOCKED = 3,
    ACCESS_FAILURE = 4,
    FAILED_WRITE = 5,
    FAILED_READ = 6,
};

class Flash {
public:
    Flash();
    int Lock();
    int Unlock();
    int Erase();
    int ReadBuffer();
    int WriteBuffer();
};
}
#endif // EVT_BOOTFLASH_HPP
