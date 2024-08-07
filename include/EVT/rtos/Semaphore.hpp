#ifndef EVT_RTOS_SEMAPHORE_
#define EVT_RTOS_SEMAPHORE_

#include <EVT/rtos/Initializable.hpp>

namespace core::rtos {

class Semaphore : Initializable {
public:
    Semaphore(ULONG initialCount, const char* name);

    UINT destroy();

    UINT get();

    UINT put();

    UINT putNotify();

    UINT prioritize();

    UINT ceilingPut();

    bool init(BytePool &pool) override;

private:
    TX_SEMAPHORE txSemaphore;
    ULONG initialCount;
    const char* name;
};

} // namespace core::rtos

#endif//EVT_RTOS_SEMAPHORE_
