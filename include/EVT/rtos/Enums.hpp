
#ifndef EVT_RTOS_ENUMS_
#define EVT_RTOS_ENUMS_

namespace core::rtos {

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

enum TXWait {
    NoWait = 0u,
    WaitForever = TX_WAIT_FOREVER
};

} //namespace core::rtos
#endif //EVT_RTOS_ENUMS_