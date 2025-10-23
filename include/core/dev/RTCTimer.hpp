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
    RTCTimer(RTC& r);

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
     */
    void startTimer(void (*irqHandler)(void* htim)) override {}

    void startTimer() override;

    void stopTimer() override;

    void reloadTimer() override;

    //
    // Since you are overriding it but not using the prescalar you should probably add a comment that it is unused. Also since it is unused, it is good practice to put (void) clockPrescaler in your definition to hint to C++ that you know it is unused and to suppress any warnings from that.
    //
    void setPeriod(uint32_t clock, uint32_t clockPrescaler) override;

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
    RTC& rtc;

    /**
     * The amount of seconds that have elapsed while the timer is running.
     * Only updates when stopTimer() is called.
     */
    uint32_t time;

    /** The amount of time it takes the timer to go off in SECONDS */
    uint32_t clockPeriod;

    /** The epoc time the clock started */
    uint32_t startTime;

    /** true if timer has been stopped */
    bool bTimerStopped;
};

} // namespace core::dev

#endif // EVT_TIMER_H
