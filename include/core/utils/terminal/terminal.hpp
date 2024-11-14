/**
 * header for terminal, finish this comment later
 */

#include <core/io/UART.hpp>
#include <core/io/pin.hpp>
#include <core/manager.hpp>
#include <menu.hpp>

namespace io = core::io;

namespace core:utils 
{
    class Terminal
    {
        public:
            /**
             * constructor for terminal object
             * takes a uart object and sets up the command line terminal
             * @param uart an instance of a UART object
             * @param baud 9600 or 115200
             */
            Terminal(io::UART& uart, utils::Menu menu);

            /**
             * Sends a provided message over UART
             * @param message a string message to send via UART
             */
            void update(char* message);

            /**
             * proccesses incoming UART messages
             */
            char*[10] recieve();

        private:
            utils::Menu menu;
            // UART instance 
            io::UART uart;

            int baud;

    }
}


