#include <EVT/utils/log.hpp>
#include <cstdio>

namespace IO = EVT::core::IO;

namespace EVT::core::log {
    /**
     * Set the UART to be used for logging
     *
     * @param uart[in] UART to be used for logging
     */
    void Logger::setUART(IO::UART *loggingUART) {
        uart = loggingUART;
    }

    /**
    * Set the maximum log level to be displayed by the logger
    *
    * @param level[in] Maximum log level to be displayed by the logger
    */
    void Logger::setLogLevel(Logger::LogLevel level) {
        maxLevel = level;
    }

    /**
     * Write logStatement to the serial logger if the logger log level reaches this level
     *
     * @param level[in] Log level of this statement
     * @param logStatement[in] String to be printed to the logger
     */
    void Logger::log(LogLevel level, const char *logStatement) {
        // If there isn't a UART interface, cannot print
        // If the logger's maxLevel is less than the level of this statement, it shouldn't print
        if (!uart || level > maxLevel)
            return;

        // Print the level of this log statement
        switch(level) {
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

        uart->printf(logStatement);
    }

    Logger LOGGER;
}