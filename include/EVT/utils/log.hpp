#ifndef EVT_LOG_
#define EVT_LOG_

#include <cstdint>
#include <EVT/io/UART.hpp>


namespace EVT::core::log {
    class Logger {
    public:
        /**
         * Set the UART to be used for logging
         *
         * @param uart[in] UART to be used for logging
         */
        void setUART(IO::UART* uart);

        /**
         * Write logStatement to the serial logger
         *
         * @param logStatement[in] String to be printed to the logger
         */
        void log(const char *logStatement);

    private:
        IO::UART* uart;
    };

    extern Logger LOGGER;
}


#endif //EVT_LOG_
