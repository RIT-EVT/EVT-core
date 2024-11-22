/**
 * header for terminal, finish this comment later
 */

#include <core/io/UART.hpp>
#include <core/io/pin.hpp>
#include <core/manager.hpp>
#include <menu.hpp>
#include <string>

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
             * returns the uart instance of this terminal
             */
            io::UART& getUART();

            /**
             * returns the menu instance that serves as the root node of the tree containing the terminal
             */
            utils::Menu getMenu();

            /**
             * Sends a provided message over UART
             * @param message a string message to send via UART
             */
            void update(std::string message);

            /**
             * proccesses incoming UART messages
             */
            std::string recieve();

        private:
            // menu instance
            utils::Menu menu;

            // UART instance 
            io::UART uart;

    }
}


