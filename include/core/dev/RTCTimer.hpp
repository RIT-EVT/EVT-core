#ifndef EVT_TIMER_H
#define EVT_TIMER_H

#include <core/dev/RTC.hpp>
#include <core/dev/Timer.hpp>
#include <core/utils/time.hpp>

namespace time = core::time;

namespace core::dev {

class RTCTimer : public Timer {
public:
    /**
     * Create instance of RTCTimer.
     * clock period defaults to 1s.
     *
     * @param r
     */
    explicit RTCTimer(RTC& r);

    /**
     * Create instance of RTCTimer.
     *
     * @param[in] r Instance of on-board
     * @param[in] clock Amount of time it takes for the time to go off in ms
     */
    RTCTimer(RTC& r, uint32_t clock);

    /**
     * not implemented.
     *
     * @param[in] irqHandler The IRQ Handler function pointer.  Sets a new interrupt handler function
     * @param[in] context The context used by the irqHandler.
     */
    void startTimer(void (*irqHandler)(void* context, void* htim), void* context) override {}

    void startTimer() override;

    void stopTimer() override;

    void reloadTimer() override;

    /**
     * Set the period for PWM.
     * Note: This does not match the dev::Timer set period, since the RTCTimer does not accept a different period.
     * This function will hide the definition from the superclass. So you can safely ignore the warning.
     * @param[in] period The period for the RTC timer.
     * @param[in] clockPrescaler UNUSED DO NOT SET.
     */
    void setPeriod(uint32_t period, uint32_t clockPrescaler = AUTO_PRESCALER);

    /**
     * Gets the time since the RTC clock began in seconds
     *
     * @return the time since the RTC clock began
     */
    [[nodiscard]] uint32_t getTime() const;

    /**
     * Gets whether the timer has gone off
     *
     * @return whether the time has gone off
     */
    [[nodiscard]] bool hasGoneOff() const;

private:
    /** Instance of on-board*/
    RTC& rtc;

    /**
     * The amount of seconds that have elapsed while the timer is running.
     * Only updates when stopTimer() is called.
     */
    uint32_t time;

    /** The amount of time it takes the timer to go off in SECONDS */
    uint32_t clockPeriod{};

    /** The epoc time the clock started */
    uint32_t startTime;

    /** true if timer has been stopped */
    bool bTimerStopped = true;
};

} // namespace core::dev

#endif // EVT_TIMER_H
