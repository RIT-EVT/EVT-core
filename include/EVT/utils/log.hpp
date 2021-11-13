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
         * Write logStatement to the serial logger if the logger log level reaches this level
         *
         * @param level[in] Log level of this statement
         * @param logStatement[in] String to be printed to the logger
         */
        void log(LogLevel level, const char *logStatement);

    private:
        IO::UART* uart;
        LogLevel minLevel;
        dev::RTC* clock;
    };

    extern Logger LOGGER;
}


#endif //EVT_LOG_
