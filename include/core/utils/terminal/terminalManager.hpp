#ifndef EVT_TERMINAL
#define EVT_TERMINAL

#include <core/io/UART.hpp>
#include <core/io/pin.hpp>
#include <core/utils/terminal/menu.hpp>
#include <core/utils/terminal/menuItem.hpp>
#include <core/utils/terminal/terminal.hpp>
#include <cstring>

namespace core::utils {

class TerminalManager {
public:
    /**
     * constructor for terminal object
     * takes a uart object and sets up the command line terminal
     * @param uart an instance of a UART object
     * @param menu head node of terminal structure
     */
    Terminal(io::UART& uart, Menu* menu);

    /**
     * returns uart instance used
     */
    io::UART& getUART() {
        return uart;
    }

    /**
     * returns main menu
     */
    Menu* getMenu() {
        return menu;
    }

    /**
     * enters the given submenu
     * @param uart the uart instance to use
     * @param args the arguments provided, used for enter callback, first index is still option string
     * @param term void pointer to the terminal this is being done inside of(optional to leave as nullptr, just here to
     * be used with callbacks)
     */
    void enterSub(io::UART& uart, char** args, void* term);
    /**
     * sets current menu/submenu to provided value
     * @param sub the submenu to replace the current one with
     */
    void setMenu(SubMenu* sub);

    /**
     * checks if the terminal is still on the main menu
     * OK FOR NOW< MAYBE MAKE IT DETECT WHEN HEAD INSTED OF USING A FLAG
     */
    bool isMain() {
        return m;
    }

    /**
     * Sends a provided message over UART
     * also replaces menu with provided menu
     * used to reset menu to desired state and notify user
     * @param message a string message to send via UART
     * @param menu the menu to replace the current menu with
     */
    void update(char* message, utils::Menu menu);

    /**
     * proccesses incoming UART messages
     * @param holder list to store tokenized strings in, should be full of "/0" to start
     */
    bool recieve(char** holder);

    /**
     * processes chosen menu item and runs its callback(if exit, exits into higher menu)
     * @param tag the user input key for the item
     */
    void process(char* tag, char** args);

    // TERMINAL specific print function
    void printTerm();

private:
    // menu instance
    Menu* menu;

    // current submenu, if there is one, nullptr otherwise
    Menu* current;

    // true if currently in main menu
    bool m;

    // uart instance this terminal is using
    core::io::UART& uart;

    // char buffer for UART entry
    char buffer[99];
};
} // namespace core::utils

#endif