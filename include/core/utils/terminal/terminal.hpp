#ifndef EVT_TERMINAL
#define EVT_TERMINAL

#include <core/io/UART.hpp>
#include <core/io/pin.hpp>
#include <core/utils/terminal/menu.hpp>
#include <core/utils/terminal/menuItem.hpp>
#include <cstring>

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
            Terminal(io::UART& uart, Menu* menu);

            io::UART& getUART()
            {
                return uart;
            }

            Menu* getMenu()
            {
                return menu;
            }

            SubMenu* getCurrent()
            {
                return current;
            }
            /**
             * sets current submenu to provided value
             * @param sub the submenu to replace the current one with
             */
            void setCurrent(SubMenu* sub);

            /**
             * checks if the terminal is still on the main menu
             */
            bool isMain(){return m;}

            /**
             * Sends a provided message over UART
             * @param message a string message to send via UART
             */
            void update(char* message);

            /**
             * proccesses incoming UART messages
             */
            void recieve(char** holder);

            void process(utils::MenuItem* holder, char* tag);

        private:
            // menu instance
            Menu* menu;

            SubMenu* current;

            bool m;

            core::io::UART& uart;

            // UART instance 

    };
}

#endif