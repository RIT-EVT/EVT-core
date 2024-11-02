
#ifndef _EVT_RTOS_Initializeable_
#define _EVT_RTOS_Initializeable_

#include <core/rtos/BytePoolBase.hpp>
#include <core/rtos/Enums.hpp>
#include <tx_api.h>

namespace core::rtos {

/**
 * Abstract class that represents a ThreadX wrapper object that must be passed into the
 * Threadx::startKernel() method as part of the initList parameter.
 */
class Initializable {
public:
    /**
     * Method that the threadx init method will call to start this object.
     *
     * @param pool The byte pool that this initializable object will be stored in.
     * @return The first error found by the function (or Success if there was no error).
     */
    virtual TXError init(BytePoolBase& pool) = 0;

    /**
     * The notify function that has been registered with this initializable object.
     * Must be public for template function reasons, but should be internally used only.
     * DO NOT CALL THIS
     */
    void (*storedNotifyFunction)(Initializable*);

    void (*storedThreadNotifyFunction)(Initializable*, uint32_t id);
};

// Template functions for notify functions

/**
 * Template function that serves as a static wrapper for the storedNotifyFunction.
 * This is only used within the Initializable class(es) and should not be used externally.
 * @tparam initializable a pointer to the object this instance of the method was created for.
 * @tparam txType the type of tx struct this instance of the method takes in.
 * @param[in] txStruct The pointer to the struct that triggered this notification function.
 */
template<Initializable* initializable, typename txType>
void txNotifyFunctionTemplate(txType* txStruct) {
    initializable->storedNotifyFunction(initializable);
}

/**
 * A template function that literally just functions as a static wrapper for the storedNotifyFunction of a thread.
 * This is only used internally to the Thread class and should never be called externally.
 * @tparam T The argument the thread takes
 * @tparam thread The specific thread this function is for
 * @param[in] threadStruct The pointer to the thread struct that triggered this notification function.
 * @param[in] id A value representing why the notification was called (thread entry, thread exit).
 */
template<Initializable* initializable>
void txThreadNotifyFunctionTemplate(TX_THREAD* threadStruct, UINT id) {
    initializable->storedThreadNotifyFunction(initializable, id);
}

} // namespace core::rtos

#endif //_EVT_RTOS_Initializeable_
