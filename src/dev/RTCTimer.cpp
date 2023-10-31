#include "EVT/dev/RTCTimer.hpp"

namespace EVT::core::DEV {

RTCTimer::RTCTimer(RTC& r) : rtc(r) {
    time = 0;
    setPeriod(1000);
    startTime = rtc.getTime();
}

RTCTimer::RTCTimer(RTC& r, uint32_t clock) : rtc(r) {
    time = 0;
    setPeriod(clock);
    startTime = rtc.getTime();
}

void RTCTimer::startTimer() {
    startTime = rtc.getTime();
    bTimerStopped = false;
}

void RTCTimer::stopTimer() {
    time += rtc.getTime() - startTime;
    bTimerStopped = true;
}

void RTCTimer::reloadTimer() {
    time = 0;
    startTime = rtc.getTime();
    bTimerStopped = false;
}

void RTCTimer::setPeriod(uint32_t clock) {
    clockPeriod = clock / 1000;
}

uint32_t RTCTimer::getTime() {
    return bTimerStopped ? time : time + rtc.getTime() - startTime;
}

bool RTCTimer::hasGoneOff() {
    return getTime() >= clockPeriod;
}
}// namespace EVT::core::DEV