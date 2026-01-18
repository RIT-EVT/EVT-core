#include <array>
#include <core/io/UART.hpp>
#include <core/io/pin.hpp>
#include <core/manager.hpp>
#include <core/utils/terminal/menu.hpp>
#include <core/utils/terminal/menuItem.hpp>
#include <core/utils/terminal/terminal.hpp>
#include <cstring>

namespace io    = core::io;
namespace utils = core::utils;

// example callback that prints a test message
void printCB(io::UART& uart, char** inputList, void* term) {
    uart.printf("\n\rTest Message\n\r");
}

// example callback that echos user input(called send because e char is taken by exit)
void sendCB(io::UART& uart, char** inputList, void* term) {
    uart.printf("\n\r");
    for (int i = 1; i < 10; i++) {
        uart.printf(inputList[i]);
        uart.printf(" ");
    }
}

// example submenu entercb
void subCB(io::UART& uart, char** inputList, void* term) {
    // enters submenu with no other behavior
    utils::Terminal* t = (utils::Terminal*) term;
    t->enterSub(uart, inputList, term);
}

int main() {
    // core setup
    core::platform::init();

    // UART setup
    io::UART& uart = io::getUART<io::Pin::UART_TX, io::Pin::UART_RX>(9600);

    // item list end item
    utils::MenuItem nul = utils::MenuItem(nullptr, nullptr, "null", "LIST END\n\r", nullptr, nullptr);

    // make some item lists
    utils::MenuItem* items[10];

    // this is really optional
    for (int i = 0; i < 10; i++) {
        items[i] = nullptr;
    }

    // list of submenu items
    utils::MenuItem* subs[3];

    // make empty main menu and terminal containing it

    utils::Menu menu     = utils::Menu(items);
    utils::Terminal term = utils::Terminal(uart, &menu);

    // void* to both for use in thier nodes
    void* m = &menu;
    void* t = &term;

    // make items to fill terminal with
    // a nullptr in the head parameter means it is part of the main menu
    utils::MenuItem mainPrint = utils::MenuItem(nullptr, t, "p", "print test\n\r", printCB, nullptr);
    utils::MenuItem mainSend  = utils::MenuItem(nullptr, t, "s", "echo\n\r", sendCB, nullptr);

    /**
     * submenu cb is exit cb struct, ctx is void* to enter function cb struct/function.
     * these are NOT responsible for moving menus, just for operations done duirng a menu move
     * this example: when we enter sub, subCB is executed, this is a cb defined in main that just runs enterCB, this is
     * just the bare minimum but it ideally would also contain your custom enterence behavior when we exit, ctx is
     * nullptr so only the normal submenu exit is used this could also be a void* to a cb struct/function
     */
    utils::SubMenu sub = utils::SubMenu(nullptr, t, "sb", "sub menu\n\r", subCB, nullptr, subs);
    void* s            = &sub;

    // more example menuitems, this time for the submenu
    utils::MenuItem subPrint = utils::MenuItem(s, t, "p", "print test\n\r", printCB, nullptr);
    utils::MenuItem subSend  = utils::MenuItem(s, t, "s", "echo\n\r", sendCB, nullptr);

    // filling a submenu
    subs[0] = &subPrint;
    subs[1] = &subSend;
    subs[2] = &nul;

    // set submenu to updated list
    sub.setItems(subs);

    // add main menu items to main menu
    items[0] = &mainPrint;
    items[1] = &mainSend;
    items[2] = &sub;
    items[3] = &nul;

    // set menu items to the updated list
    menu.newItems(items);

    // usage instructions to print before terminal fully starts
    uart.printf("Usage: flagChar argument argument | max 9 arguments\n\r");

    // forever while loop to constantly wait for user input inside of recieve()
    // create empty list of strings
    char* inputList[10];

    // empty string to store input for item selection
    char* tag;

    while (true) {
        // print current state of terminal
        term.printTerm();

        // recieve input
        term.recieve(inputList);

        // set tag to inputted selection string
        tag = inputList[0];

        // process input, this uses input to find chosen item, and execute/enter it
        term.process(tag, inputList);
    }

    // this should never happen
    return 0;
}