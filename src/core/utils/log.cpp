#include <core/utils/log.hpp>
#include <cstdarg>
#include <cstdio>

namespace IO = EVT::core::IO;

namespace EVT::core::log {
/**
 * Set the UART to be used for logging
 *
 * @param uart[in] UART to be used for logging
 */
void Logger::setUART(IO::UART* loggingUART) {
#ifdef EVT_CORE_LOG_ENABLE
    uart = loggingUART;
#endif
}

/**
* Set the minimum log level to be displayed by the logger
*
* @param level[in] Minimum log level to be displayed by the logger
*/
void Logger::setLogLevel(Logger::LogLevel level) {
#ifdef EVT_CORE_LOG_ENABLE
    minLevel = level;
#endif
}

/**
 * Set the clock to be used for timestamps
 *
 * @param rtc Clock to be used for timestamps
 */
void Logger::setClock(dev::RTC* rtc) {
#ifdef EVT_CORE_LOG_ENABLE
    clock = rtc;
#endif
}

/**
 * Write the formatted string to the serial logger if the logger log level
 * reaches this level
 *
 * @param level[in] Log level of this statement
 * @param format[in] Format string to be logged
 * @param ...[in] Variables to be formatted into the log statement
 */
void Logger::log(LogLevel level, const char* format, ...) {
#ifdef EVT_CORE_LOG_ENABLE
    // If there isn't a UART interface, cannot print
    // If the level of this statement is less than the logger's minLevel,
    // shouldn't print
    if (!uart || level < minLevel)
        return;

    // If the clock has been set, print the timestamp
    if (clock) {
        uint32_t time = clock->getTime();
        uart->printf("%d ", time);
    }

    // Print the level of this log statement
    switch (level) {
    case LogLevel::DEBUG:
        uart->printf("DEBUG::");
        break;
    case LogLevel::INFO:
        uart->printf("INFO::");
        break;
    case LogLevel::WARNING:
        uart->printf("WARNING::");
        break;
    case LogLevel::ERROR:
        uart->printf("ERROR::");
        break;
    }

    // Initialize variable argument list
    va_list args;
    va_start(args, format);

    // Apply print formatting from the variable argument list to format and
    // print it
    char logString[200];
    vsprintf(logString, format, args);
    uart->printf("%s\r\n", logString);

    // Closes the variable argument list
    va_end(args);
#endif
}

Logger LOGGER;
}// namespace EVT::core::log
