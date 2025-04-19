/**
 *
 */
#include <core/io/UART.hpp>
#include <core/io/pin.hpp>
#include <core/utils/terminal/menu.hpp>
#include <core/utils/terminal/menuItem.hpp>
#include <core/utils/terminal/terminalManager.hpp>
#include <cstring>

namespace io = core::io;

namespace core::utils {
Terminal::Terminal(io::UART& uart, utils::Menu* menu) : menu(menu), uart(uart) {
    // start message
    uart.printf("\n\rStarting Terminal...\n\r");
    // set main menu flag to true
    m = true;
}

void Terminal::update(char* message, utils::Menu m) {
    // replace menu with provided
    menu->replace(m);
    // print spacer lines
    for (int i = 0; i < 5; i++) {
        uart.printf("\n\r");
    }
    // print
    uart.printf(message);
}

void Terminal::setCurrent(SubMenu* sub) {
    // sets current to input, sets main menu flag to false
    current = sub;
    m       = false;
}

bool Terminal::recieve(char* holder[10]) {
    // fill buffer with null chars
    for (int i = 0; i < 99; i++) {
        buffer[i] = '\0';
    }
    // fill buffer
    uart.gets(buffer, 99);

    // tokenize
    holder[0] = strtok(buffer, " ");
    for (int i = 1; i < 10; i++) {
        holder[i] = strtok(NULL, " ");
    }
    // return true, this is placeholder for ensuring a complete message
    return true;
    // for(int i = 0; i < 10; i++)
    // {
    //     holder[i] = argN[i];
    // }
}

// TERMINAL specific print function
void Terminal::printTerm() {
    // print header and some info
    uart.printf("\n\r");
    uart.printf("Terminal:\n\r");
    // uart.printf("is main: ");
    // if (m) {
    //     uart.printf("true\n\r");
    // } else {
    //     uart.printf("false\n\r");
    // }
    uart.printf(current->getText()); //this will eventually be changed to brint the nodes above current up to the head as well like head>between>current
    uart.printf("===================\n\r");
    // print menu
    if (m) {
        menu->printStr(uart);
    }
    // or submenu
    else {
        current->printMStr(uart);
        uart.printf("q|QUIT\n\r");
    }
}

void Terminal::enterSub(io::UART& uart, char** args, void* term) {
    utils::MenuItem** items = menu->getItems();
    char* name              = args[0];
    int c                   = menu->getCount();
    utils::MenuItem* item;
    // find chosen item
    for (int i = 0; i < c; i++) {
        item = items[i];
        if (strcmp(name, item->getOption()) == 0) {
            // cast item to submenu and place in current
            setCurrent((utils::SubMenu*) item);
            break;
        }
    }
}

void Terminal::process(char* tag, char** args) {
    // different behavior for menu and submenu
    utils::MenuItem* holder;
    if (m) {
        // find item in menu
        utils::MenuItem** subitemsM = menu->getItems();

        for (int i = 0; i < menu->getCount(); i++) {
            char* op = subitemsM[i]->getOption();

            if (strcmp(op, "null") == 0) {
                holder = nullptr;
                break;
            } else if (strcmp(tag, op) == 0) {
                holder = (subitemsM[i]);
                break;
            }
        }
    } else {
        // find item in submenu
        utils::MenuItem** subitemsC = current->getItems();

        for (int i = 0; i < menu->getCount(); i++) {
            char* op = subitemsC[i]->getOption();
            if (strcmp(op, "null") == 0) {
                holder = nullptr;
                break;
            }
            if (strcmp(tag, op) == 0) {
                holder = (subitemsC[i]);
                break;
            }
        }
    }

    // check if quit flag, if quit set main menu flag to true
    // this brings us back to main menu, current sill still have a value but this gets overwritten next menu move
    if (strcmp(tag, "q") == 0) {
        m = true;
    }

    // if quit flag, kick out of process,
    if (holder == nullptr || strcmp(holder->getOption(), "q") == 0) {
        return;
    }

    // get and run callback of chosen item
    callback_t cb = holder->getcb();

    cb(uart, args, (void*) this);
}
} // namespace core::utils
