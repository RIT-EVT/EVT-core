
#ifndef EVT_RTOS_THREADX_
#define EVT_RTOS_THREADX_

#include <EVT/rtos/Initializable.hpp>
#include <cstdint>

namespace core::rtos {

/**
 * Allocates a bytepool to store all of the Initializable objects passed into
 * this function, initializes them, and begins the threadx kernel.
 *
 * @param initList the list of Initializeable objects that must be initialized
 * before the threadx kernel begins.
 * @param length the length of initList.
 * @param poolptr a reference to the bytepool to store the information of the initializable objects in.
 */
void init(Initializable* initList, std::size_t length, BytePoolBase&poolptr);

enum TXError {
    Success = 0x00u,
    Deleted,
    PoolError,
    PtrError,
    WaitError,
    SizeError,
    GroupError,
    NoEvents,
    OptionError,
    QueueError,
    QueueEmpty,
    QueueFull,
    SemaphoreError,
    NoInstance,
    ThreadError,
    PriorityError,
    NoMemory = 0x10u,
    StartError = 0x10u,
    DeleteError,
    ResumeError,
    CallerError,
    SuspendError,
    TimerError,
    TickError,
    ActivateError,
    ThreshError,
    SuspendLifted,
    WaitAborted,
    WaitAbortError,
    MutexError,
    NotAvailable,
    NotOwned,
    InheritError,
    NotDone,
    CeilingExceeded,
    InvalidCeiling,
    FeatureNotEnabled = 0xFFu
};

} // namespace core::rtos

#endif//EVT_RTOS_THREADX_
