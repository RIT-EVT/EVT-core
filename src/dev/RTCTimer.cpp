#include "EVT/dev/RTCTimer.hpp"

namespace EVT::core::DEV {

RTCTimer::RTCTimer(RTC& rtc) {
    m_rtc = &rtc;
    m_time = 0;
    setPeriod(1000);
    m_startTime = m_rtc->getTime();
}

RTCTimer::RTCTimer(RTC& rtc, uint32_t clockPeriod) {
    m_rtc = &rtc;
    m_time = 0;
    setPeriod(clockPeriod);
    m_startTime = m_rtc->getTime();
}

void RTCTimer::startTimer() {
    m_startTime = m_rtc->getTime();
    m_bTimerStopped = false;
}

void RTCTimer::stopTimer() {
    m_time += m_rtc->getTime() - m_startTime;
    m_bTimerStopped = true;
}

void RTCTimer::reloadTimer() {
    m_time = 0;
    m_startTime = m_rtc->getTime();
    m_bTimerStopped = false;
}

void RTCTimer::setPeriod(uint32_t clockPeriod) {
    m_clockPeriod = clockPeriod / 1000;
}

uint32_t RTCTimer::getTime() {
    return m_bTimerStopped ? m_time : m_time + m_rtc->getTime() - m_startTime;
}

bool RTCTimer::hasGoneOff() {
    return getTime() >= m_clockPeriod;
}

}// namespace EVT::core::DEV
