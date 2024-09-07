
#ifndef EVT_RTOS_ENUMS_
#define EVT_RTOS_ENUMS_

namespace core::rtos {

/**
 * Enum representing the possible range of Error Codes from ThreadX.\n\n
 *
 * Note that it is technically possible for methods that return TXError
 * to return a value outside of the enum, so when this enum is used in a switch statement
 * there ALWAYS should be an "default" case.
 */
enum TXError {
    Success = TX_SUCCESS,
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
    NoMemory = TX_NO_MEMORY,
    StartError = TX_START_ERROR,
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
    FeatureNotEnabled = TX_FEATURE_NOT_ENABLED
};

/**
 * Enum to shorthand No Wait and Wait Forever wait options in TX.
 */
enum TXWait {
    NoWait = TX_NO_WAIT,
    WaitForever = TX_WAIT_FOREVER
};

} //namespace core::rtos
#endif //EVT_RTOS_ENUMS_