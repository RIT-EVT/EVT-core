#ifndef EVT_TIMER_H
#define EVT_TIMER_H

#include <EVT/dev/RTC.hpp>
#include <EVT/dev/Timer.hpp>
#include <EVT/utils/time.hpp>

namespace time = EVT::core::time;

namespace EVT::core::DEV {

class RTCTimer : public Timer {
public:
    /**
     * Create instance of RTCTimer.
     * clock period defaults to 1s.
     *
     * @param rtc
     */
    RTCTimer(RTC& rtc);

    /**
     * Create instance of RTCTimer.
     *
     * @param rtc Instance of on-board
     * @param clockPeriod Amount of time it takes for the time to go off in ms
     */
    RTCTimer(RTC& rtc, uint32_t clockPeriod);

    /**
     * not implemented.
     *
     * @param irqHandler[in] The IRQ Handler function pointer.  Sets a new interrupt handler function
     */
    void startTimer(void (*irqHandler)(void* htim)) override {}

    /**
     * Starts the given timer.
     */
    void startTimer() override;

    /**
     * Stops the current timer from running.  Does not complete its current counting sequence.
     */
    void stopTimer() override;

    /**
     * Resets the timer counter.
     */
    void reloadTimer() override;

    /**
     * Set the clock period for the timer.  Will stop the timer, re-initialize the device with the updated period.
     * You must call startTimer again to continue timer operation.
     *
     * @param clockPeriod[in] the clock period in MILLISECONDS.  An interrupt will be triggered at that frequency.
     */
    void setPeriod(uint32_t clockPeriod) override;

    /**
     * Gets the time since the RTC clock began in seconds
     *
     * @return the time since the RTC clock began
     */
    uint32_t getTime();

    /**
     * Gets whether the timer has gone off
     *
     * @return whether the time has gone off
     */
    bool hasGoneOff();

private:
    /** Instance of on-board*/
    RTC* m_rtc;

    /**
     * The amount of seconds that have elapsed while the timer is running.
     * Only updates when stopTimer() is called.
     */
    uint32_t m_time;

    /** The amount of time it takes the timer to go off in SECONDS */
    uint32_t m_clockPeriod;

    /** The epoc time the clock started */
    uint32_t m_startTime;

    /** true if timer has been stopped */
    bool m_bTimerStopped;
};

}// namespace EVT::core::DEV

#endif//EVT_TIMER_H
