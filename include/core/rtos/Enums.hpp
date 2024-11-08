#ifndef _EVT_RTOS_ENUMS_
#define _EVT_RTOS_ENUMS_

#include <tx_api.h>

namespace core::rtos {

/**
 * Enum representing the possible error codes returned by ThreadX
 * \n\n
 *
 * Note: It is technically possible for methods that return TXError to
 * return a value outside of the enum, so when this enum is used in a switch
 * statement there ALWAYS should be an "default" case.
 */
enum TXError {
    SUCCESS = TX_SUCCESS,
    DELETED,
    POOL_ERROR,
    PTR_ERROR,
    WAIT_ERROR,
    SIZE_ERROR,
    GROUP_ERROR,
    NO_EVENTS,
    OPTION_ERROR,
    QUEUE_ERROR,
    QUEUE_EMPTY,
    QUEUE_FULL,
    SEMAPHORE_ERROR,
    NO_INSTANCE,
    THREAD_ERROR,
    PRIORITY_ERROR,
    NO_MEMORY   = TX_NO_MEMORY,
    START_ERROR = TX_START_ERROR,
    DELETE_ERROR,
    RESUME_ERROR,
    CALLER_ERROR,
    SUSPEND_ERROR,
    TIME_ERROR,
    TICK_ERROR,
    ACTIVATE_ERROR,
    THRESH_ERROR,
    SUSPEND_LIFTED,
    WAIT_ABORTED,
    WAIT_ABORT_ERROR,
    MUTEX_ERROR,
    NOT_AVAILABLE,
    NOT_OWNED,
    INHERIT_ERROR,
    NOT_DONE,
    CEILING_EXCEEDED,
    INVALID_CEILING,
    FEATURE_NOT_ENABLED = TX_FEATURE_NOT_ENABLED
};

/**
 * Enum that represents the states a thread can be in
 */
enum TXThreadState {
    READ = TX_READY,
    COMPLETED,
    TERMINATED,
    SUSPENDED,
    SLEEP,
    QUEUE_SUSPENDED,
    SEMAPHORE_SUSPENDED,
    EVENTFLAG,
    BLOCKMEMORY,
    BYTEMEMORY,
    IO_DRIVER,
    FILE,
    TCP_IP,
    MUTEX_SUSPENDED,
    PRIORITY_CHANGE
};

/**
 * Enum to shorthand No Wait and Wait Forever wait options in TX
 */
enum TXWait {
    NO_WAIT      = TX_NO_WAIT,
    WAIT_FOREVER = TX_WAIT_FOREVER
};

} // namespace core::rtos
#endif // _EVT_RTOS_ENUMS_