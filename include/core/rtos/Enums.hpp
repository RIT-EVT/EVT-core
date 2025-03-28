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
 * statement there ALWAYS should be a "default" case.
 */
enum TXError {
    TXE_SUCCESS             = TX_SUCCESS,
    TXE_DELETED             = TX_DELETE_ERROR,
    TXE_POOL_ERROR          = TX_POOL_ERROR,
    TXE_PTR_ERROR           = TX_PTR_ERROR,
    TXE_WAIT_ERROR          = TX_WAIT_ERROR,
    TXE_SIZE_ERROR          = TX_SIZE_ERROR,
    TXE_GROUP_ERROR         = TX_GROUP_ERROR,
    TXE_NO_EVENTS           = TX_NO_EVENTS,
    TXE_OPTION_ERROR        = TX_OPTION_ERROR,
    TXE_QUEUE_ERROR         = TX_QUEUE_ERROR,
    TXE_QUEUE_EMPTY         = TX_QUEUE_EMPTY,
    TXE_QUEUE_FULL          = TX_QUEUE_FULL,
    TXE_SEMAPHORE_ERROR     = TX_SEMAPHORE_ERROR,
    TXE_NO_INSTANCE         = TX_NO_INSTANCE,
    TXE_THREAD_ERROR        = TX_THREAD_ERROR,
    TXE_PRIORITY_ERROR      = TX_PRIORITY_ERROR,
    TXE_NO_MEMORY           = TX_NO_MEMORY,
    TXE_START_ERROR         = TX_START_ERROR,
    TXE_DELETE_ERROR        = TX_DELETE_ERROR,
    TXE_RESUME_ERROR        = TX_RESUME_ERROR,
    TXE_CALLER_ERROR        = TX_CALLER_ERROR,
    TXE_SUSPEND_ERROR       = TX_SUSPEND_ERROR,
    TXE_TIME_ERROR          = TX_TIMER_ERROR,
    TXE_TICK_ERROR          = TX_TICK_ERROR,
    TXE_ACTIVATE_ERROR      = TX_ACTIVATE_ERROR,
    TXE_THRESH_ERROR        = TX_THRESH_ERROR,
    TXE_SUSPEND_LIFTED      = TX_SUSPEND_LIFTED,
    TXE_WAIT_ABORTED        = TX_WAIT_ABORTED,
    TXE_WAIT_ABORT_ERROR    = TX_WAIT_ABORT_ERROR,
    TXE_MUTEX_ERROR         = TX_MUTEX_ERROR,
    TXE_NOT_AVAILABLE       = TX_NOT_AVAILABLE,
    TXE_NOT_OWNED           = TX_NOT_OWNED,
    TXE_INHERIT_ERROR       = TX_INHERIT_ERROR,
    TXE_NOT_DONE            = TX_NOT_DONE,
    TXE_CEILING_EXCEEDED    = TX_CEILING_EXCEEDED,
    TXE_INVALID_CEILING     = TX_INVALID_CEILING,
    TXE_FEATURE_NOT_ENABLED = TX_FEATURE_NOT_ENABLED
};

/**
 * Enum that represents the states a thread can be in
 */
enum TXThreadState {
    TXS_READY               = TX_READY,
    TXS_COMPLETED           = TX_COMPLETED,
    TXS_TERMINATED          = TX_TERMINATED,
    TXS_SUSPENDED           = TX_SUSPENDED,
    TXS_SLEEP               = TX_SLEEP,
    TXS_QUEUE_SUSPENDED     = TX_QUEUE_SUSP,
    TXS_SEMAPHORE_SUSPENDED = TX_SEMAPHORE_SUSP,
    TXS_EVENTFLAG           = TX_EVENT_FLAG,
    TXS_BLOCKMEMORY         = TX_BLOCK_MEMORY,
    TXS_BYTEMEMORY          = TX_BYTE_MEMORY,
    TXS_IO_DRIVER           = TX_IO_DRIVER,
    TXS_FILE                = TX_FILE,
    TXS_TCP_IP              = TX_TCP_IP,
    TXS_MUTEX_SUSPENDED     = TX_MUTEX_SUSP,
    TXS_PRIORITY_CHANGE     = TX_PRIORITY_CHANGE
};

/**
 * Enum to shorthand No Wait and Wait Forever wait options in TX
 */
enum TXWait {
    TXW_NO_WAIT      = TX_NO_WAIT,
    TXW_WAIT_FOREVER = TX_WAIT_FOREVER
};

} // namespace core::rtos
#endif // _EVT_RTOS_ENUMS_