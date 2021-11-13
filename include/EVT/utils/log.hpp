#ifndef EVT_LOG_
#define EVT_LOG_

#include <cstdint>
#include <EVT/io/UART.hpp>
#include <EVT/dev/RTC.hpp>

namespace dev = EVT::core::DEV;

namespace EVT::core::log {
    class Logger {
    public:
        /**
        * The level at which messages should be logged
        */
        enum class LogLevel {
            DEBUG = 0u,
            INFO = 1u,
            WARNING = 2u,
            ERROR = 3u,
        };

        /**
         * Set the UART to be used for logging
         *
         * @param uart[in] UART to be used for logging
         */
        void setUART(IO::UART* uart);

        /**
         * Set the minimum log level to be displayed by the logger
         *
         * @param level[in] Minimum log level to be displayed by the logger
         */
        void setLogLevel(LogLevel level);

        /**
         * Set the clock to be used for timestamps
         *
         * @param rtc Clock to be used for timestamps
         */
        void setClock(dev::RTC* rtc);

        /**
         * Write the formatted string to the serial logger if the logger log level reaches this level
         *
         * @param level[in] Log level of this statement
         * @param format[in] Format string to be logged
         * @param ...[in] Variables to be formatted into the log statement
         */
        void log(LogLevel level, const char* format, ...);

    private:
        IO::UART* uart;
        LogLevel minLevel;
        dev::RTC* clock;
    };

    // Ensures exactly 1 instance of LOGGER is created instead of creating a new one each time this
    // header is included
    extern Logger LOGGER;
}


#endif //EVT_LOG_
