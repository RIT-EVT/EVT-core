#ifndef _EVT_TIMER_HPP
#define _EVT_TIMER_HPP

#include <core/rtos/Initializable.hpp>
#include <type_traits>

namespace core::rtos {

/**
 * Template class that wraps a ThreadX Timer. Timers wait for the specified amount of time and then call an expiration
 * function that has been passed to them./n/n
 *
 * This class extends Initializable, and like all other Initializable classes must be passed into the
 * Threadx::startKernel() method as part of the initList parameter.
 * @tparam T what type of data the timer's expiration function will take. Should be at most 32 bits. (so generally
 * should be a pointer).
 */
template<typename T, typename = typename std::enable_if<sizeof(T) <= 4>::type>
class Timer : public Initializable {
public:
    /**
     * Construct a Timer object, but does not initialize it. Must call init before using \n\n
     *
     * NOTE: T (the template parameter) should only be 32 bits. if it is any bigger things will break.
     *
     * @param[in] name Name of this timer, should be no longer than INITIALIZABLE_NAME_MAX_LENGTH bytes.
     * The name is copied into this object
     * @param[in] expirationFunction The function the timer will call when it expires
     * @param[in] expirationInput The input to the expiration function
     * @param[in] initialTicks How long (in ticks) this timer will wait after it is activated
     * @param[in] rescheduleTicks How long (in ticks) this timer will wait between calls to the function after the
     * first (if 0, timer will be a one-shot and run only once)
     * @param[in] autoActivate If the timer will activate upon being initialized. If false, the timer will wait to be
     * externally activated
     */
    Timer(char* name, void (*expirationFunction)(T), T expirationInput, uint32_t initialTicks, uint32_t rescheduleTicks,
          bool autoActivate)
        : txTimer(), Initializable(name), expirationFunction(expirationFunction), expirationInput(expirationInput),
          initialTicks(initialTicks), rescheduleTicks(rescheduleTicks), autoActivate(autoActivate) {}

    /**
     * Timer deconstructor. Deactivate and delete the timer
     */
    ~Timer() {
        tx_timer_deactivate(&txTimer);
        tx_timer_delete(&txTimer);
    }

    TXError init(BytePoolBase& pool) override {
        uint32_t errorCode = tx_timer_create(&txTimer,
                                             name,
                                             (void (*)(ULONG)) expirationFunction,
                                             (ULONG) expirationInput,
                                             initialTicks,
                                             rescheduleTicks,
                                             (uint32_t) autoActivate);
        return static_cast<TXError>(errorCode);
    }

    /**
     * Activate the timer
     *
     * @return The first error found by the function or TXE_SUCCESS if there was no error
     */
    TXError activate() {
        uint32_t errorCode = tx_timer_activate(&txTimer);
        return static_cast<TXError>(errorCode);
    }

    /**
     * Deactivate the timer
     *
     * @return The first error found by the function or TXE_SUCCESS if there was no error
     */
    TXError deactivate() {
        uint32_t errorCode = tx_timer_deactivate(&txTimer);
        return static_cast<TXError>(errorCode);
    }

    /**
     * Change the initialTicks and rescheduleTicks of the function to new values
     *
     * @param[in] newInitialTicks the new value for the initialTicks of the timer
     * @param[in] newRescheduleTicks the new value for the rescheduleTicks of the timer
     * @return The first error found by the function or TXE_SUCCESS if there was no error
     */
    TXError change(uint32_t newInitialTicks, uint32_t newRescheduleTicks) {
        uint32_t errorCode = tx_timer_change(&txTimer, newInitialTicks, newRescheduleTicks);
        return static_cast<TXError>(errorCode);
    }

    /**
     * Get whether the timer is currently active (i.e. running)
     *
     * @param[out] active The returned active state
     * @return The first error found by the function or TXE_SUCCESS if there was no error
     */
    TXError isActive(bool* active) {
        uint32_t errorCode = tx_timer_info_get(&txTimer, nullptr, (UINT*) active, nullptr, nullptr, nullptr);
        return static_cast<TXError>(errorCode);
    }

    /**
     * Get the remaining ticks before the next call of the expiration function
     *
     * @param[out] remainingTicks The returned remaining ticks
     * @return The first error found by the function or TXE_SUCCESS if there was no error
     */
    TXError getRemainingTicks(uint32_t* remainingTicks) {
        uint32_t errorCode = tx_timer_info_get(&txTimer, nullptr, nullptr, remainingTicks, nullptr, nullptr);
        return static_cast<TXError>(errorCode);
    }

    /**
     * Get the current reschedule ticks of the timer
     *
     * @param[out] rescheduleTicks The returned reschedule ticks
     * @return The first error found by the function or TXE_SUCCESS if there was no error
     */
    TXError getRescheduleTicks(uint32_t* rescheduleTicks) {
        uint32_t errorCode = tx_timer_info_get(&txTimer, nullptr, nullptr, nullptr, rescheduleTicks, nullptr);
        return static_cast<TXError>(errorCode);
    }

private:
    /** The threadx struct that represents this object in the threadx kernel */
    TX_TIMER txTimer;

    /** The expiration function registered to this timer */
    void (*expirationFunction)(T);

    /** The input to the expiration function */
    T expirationInput;

    /** How long (in ticks) the timer will wait to call the expiration function after it has been activated */
    uint32_t initialTicks;

    /** How long (in ticks) the timer will wait to call the expiration function after it has been called once */
    uint32_t rescheduleTicks;

    /** If the timer activates after being initialized. If false, the timer will have to be externally activated */
    bool autoActivate;
};

} // namespace core::rtos

#endif // _EVT_TIMER_HPP
