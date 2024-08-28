#ifndef INCLUDE_EVT_UTILS_LOG_HPP_
#define INCLUDE_EVT_UTILS_LOG_HPP_

#include <core/dev/RTC.hpp>
#include <core/io/UART.hpp>
#include <cstdint>

namespace dev = core::dev;

namespace core::log {
/** To enable the logger, compile with "cmake -D EVT_CORE_LOG_ENABLE" */
class Logger {
public:
    /**
     * The level at which messages should be logged
     */
    enum class LogLevel
    {
        DEBUG   = 0u,
        INFO    = 1u,
        WARNING = 2u,
        ERROR   = 3u,
    };

    /**
     * Set the UART to be used for logging
     *
     * @param[in] uart UART to be used for logging
     */
    void setUART(io::UART* uart);

    /**
     * Set the minimum log level to be displayed by the logger
     *
     * @param[in] level Minimum log level to be displayed by the logger
     */
    void setLogLevel(LogLevel level);

    /**
     * Set the clock to be used for timestamps
     *
     * @param[in] rtc Clock to be used for timestamps
     */
    void setClock(dev::RTC* rtc);

    /**
     * Write the formatted string to the serial logger if the logger log level
     * reaches this level
     *
     * @param[in] level Log level of this statement
     * @param[in] format Format string to be logged
     * @param[in] ... Variables to be formatted into the log statement
     */
    void log(LogLevel level, const char* format, ...);

private:
    /** UART to be used for logging */
    io::UART* uart;
    /** Minimum log level to be displayed by the logger */
    LogLevel minLevel;
    /** Clock to be used for timestamps */
    dev::RTC* clock;
};

// extern keyword ensures exactly 1 instance of LOGGER is created instead of
// creating a new one each time this header is included
/** Global Logger instance */
extern Logger LOGGER;

} // namespace core::log

#endif // INCLUDE_EVT_UTILS_LOG_HPP_
