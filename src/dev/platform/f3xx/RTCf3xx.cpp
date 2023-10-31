#include <EVT/dev/platform/f3xx/RTCf3xx.hpp>
#include <HALf3/stm32f3xx_hal_rtc.h>

namespace time = EVT::core::time;

namespace EVT::core::DEV {

RTCf3xx::RTCf3xx() : halRTC{
    RTC1,
    {
        // Numbers generated from STMCubeMX
        RTC_HOURFORMAT_24,
        127,
        255,
        RTC_OUTPUT_DISABLE,
        RTC_OUTPUT_TYPE_PUSHPULL,
        RTC_OUTPUT_POLARITY_HIGH,
    },
} {
    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

    // Initialize the peripherals
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;

    // Set the clock to LSE
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE;
    RCC_OscInitStruct.LSEState = RCC_LSE_ON;
    PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;

    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

    __HAL_RCC_RTC_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();

    HAL_RTC_Init(&halRTC);
}

void RTCf3xx::getTime(EVT::core::time::TimeStamp& time) {
    RTC_DateTypeDef rtcDate;
    RTC_TimeTypeDef rtcTime;

    // Get clock time information
    HAL_RTC_GetTime(&halRTC, &rtcTime, RTC_FORMAT_BIN);
    time.hour = rtcTime.Hours;
    time.minute = rtcTime.Minutes;
    time.second = rtcTime.Seconds;

    // Get date information
    HAL_RTC_GetDate(&halRTC, &rtcDate, RTC_FORMAT_BIN);
    time.year = 2000 + rtcDate.Year;
    time.month = rtcDate.Month;
    time.day = rtcDate.Date;
}

uint32_t RTCf3xx::getTime() {
    time::TimeStamp ts{};
    getTime(ts);

    uint32_t y = ts.year;
    uint32_t m = ts.month;
    uint32_t d = ts.day;
    uint32_t h = ts.hour;
    uint32_t min = ts.minute;
    uint32_t s = ts.second;

    // Get number of days since epoch
    // This algorithm is used to convert a date/time to an epoch.
    // This algorithm was taken from http://howardhinnant.github.io/date_algorithms.html
    y -= m <= 2;
    const uint32_t era = (y >= 0 ? y : y - 399) / 400;
    const uint32_t yoe = static_cast<uint32_t>(y - era * 400);
    const uint32_t doy = (153 * (m + (m > 2 ? -3 : 9)) + 2) / 5 + d - 1;
    const uint32_t doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
    uint32_t time = era * 146097 + static_cast<int32_t>(doe) - 719468;

    // Convert to seconds
    time *= 86400;

    // Add current number of seconds in day
    time += (h * 3600) + (min * 60) + s;

    return time;
}

void RTCf3xx::setTime(EVT::core::time::TimeStamp& time) {
    RTC_DateTypeDef rtcDate;
    RTC_TimeTypeDef rtcTime;

    rtcDate.WeekDay = time.day;
    rtcDate.Month = static_cast<uint8_t>(time.month);
    rtcDate.Date = time.day;
    rtcDate.Year = time.year % 100;

    // RTC struct for the time
    rtcTime.Hours = time.hour;
    rtcTime.Minutes = time.minute;
    rtcTime.TimeFormat = RTC_HOURFORMAT_24;
    rtcTime.Seconds = time.second;
    rtcTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    rtcTime.StoreOperation = RTC_STOREOPERATION_RESET;

    HAL_RTC_SetDate(&halRTC, &rtcDate, RTC_FORMAT_BIN);
    HAL_RTC_SetTime(&halRTC, &rtcTime, RTC_FORMAT_BIN);
}

}// namespace EVT::core::DEV