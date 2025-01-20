#ifndef EVT_TERMINAL
#define EVT_TERMINAL

#include <core/io/UART.hpp>
#include <core/io/pin.hpp>
#include <core/manager.hpp>
#include <core/utils/terminal/menu.hpp>
#include <string>
#include <array>


namespace core::utils
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
            utils::SubMenu getCurrent();

            /**
             * sets current submenu to provided value
             * @param sub the submenu to replace the current one with
             */
            void setCurrent(SubMenu sub);

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
            std::array<std::string, 10> recieve();

        protected:
            // menu instance
            utils::Menu menu;

            utils::SubMenu current;

            bool m;

            core::io::UART& uart;

            // UART instance 

    };
}

#endif