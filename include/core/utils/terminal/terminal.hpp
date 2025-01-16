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
             * returns the current page of the menu, only if the menu is not currently on the main page 
             * */
            utils::MenuItem getCurrent();

            /**
             * sets current submenu to provided value
             */
            void setCurrent();

            /**
             * checks if the terminal is still on the main menu
             */
            bool isMain();

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

            utils::SubMenu current;

            bool m;

            // UART instance 
            io::UART uart;

    }
}


