#include <EVT/utils/log.hpp>

namespace IO = EVT::core::IO;

namespace EVT::core::log {
    /**
     * Set the UART to be used for logging
     *
     * @param uart[in] UART to be used for logging
     */
    void Logger::setUART(IO::UART* loggingUART) {
        uart = loggingUART;
    }

    /**
     * Writes the logStatement to the serial logger
     *
     * @param logStatement[in] String to be printed to the logger
     */
    void Logger::log(const char *logStatement) {
        uart->printf(logStatement);
    }

    Logger LOGGER;
}