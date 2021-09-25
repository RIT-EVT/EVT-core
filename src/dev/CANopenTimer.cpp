#include <EVT/dev/CANopenTimer.hpp>


namespace {
    EVT::core::DEV::RTC* rtc;
    uint32_t frequency;
    uint32_t targetReload;
    uint32_t startTime;
}

/**
 * Initiailize the timer driver.
 */
static void timerInit(uint32_t freq) {
    frequency = freq;
}

static void timerReload(uint32_t reload) {
    targetReload = reload;
}

/**
 * Start the "timer" running
 */
static void timerStart(void) {
    startTime = rtc->getTime();
}

/**
 * Return true if the timer has gone off
 */
static uint8_t timerUpdate(void) {
    uint32_t targetTime = startTime + (targetReload / frequency);
    return rtc->getTime() >= targetTime;
}

/**
 * Get the reload value that was set
 */
static uint32_t timerDelay(void) {
    return targetReload;
}

/**
 * Stop the timer, currently does nothing.
 */
static void timerStop(void) {
    
}

namespace EVT::core::DEV {

    void getCANopenTimerDriver(RTC& rtcIntf, CO_IF_TIMER_DRV* timerDriver) {
        rtc = &rtcIntf;
        
        timerDriver->Init = timerInit;
        timerDriver->Reload = timerReload;
        timerDriver->Delay = timerDelay;
        timerDriver->Stop = timerStop;
        timerDriver->Start = timerStart;
        timerDriver->Update = timerUpdate;
    }

}  // namespace EVT::core::IO
