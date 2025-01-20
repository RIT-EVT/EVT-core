#include <core/utils/terminal/terminal.hpp>
#include <core/utils/terminal/menu.hpp>
#include <core/utils/terminal/menuItem.hpp>
#include <core/io/UART.hpp>
#include <core/io/pin.hpp>
#include <core/manager.hpp>
#include <string>

namespace io   = core::io;
namespace utils  = core::utils;

int main()
{
    //core setup
    core::platform::init();

    //UART setup
    io::UART& uart = io::getUART<io::Pin::UART_TX, io::Pin::UART_RX>(9600);

    //make some items
    utils::Terminal::MenuItem print = MenuItem::MenuItem("p","Print, takes only key", printCB, nullptr);
    utils::Terminal::MenuItem send = MenuItem::MenuItem("s","Send, takes key and message", sendCB, nullptr);
    utils::Terminal::SubMenu sub = MenuItem::SubMenu("b", "SubMenu", enterCB, nullptr, {print,send});
    utils::Terminal::MenuItem[2] items = {print, send, sub};
    utils::Terminal::Menu menu = Menu::Menu(items);
    utils::Terminal::Terminal term = Terminal::Terminal(uart, menu);

    printTerm(uart, term);
    
    
    while(true)
    {
        utils::Terminal::MenuItem[10] items;

        int c = 0;

        if(term.isMain())
        {
            items = term.getMenu().getItems();
            c = term.getMenu().getCount();
            term.update(term.getMenu().toStr());
        }
        else
        {
            items = term.getCurrent().getItems();
            c = term.getCurrent().getCount();
            term.update(term.getCurrent().toStr());
        }
        
        std::string[10] inputList = term.recieve();
        std::string tag = inputList[0];
        std::string[10] args = inputList[1:];

        void (*cb)(io::UART, void*, utils::Terminal::Terminal);

        for(int i = 0; i < c; i ++)
        {
            if(tag = items[i].getOption())
            {
                cb = items[i].getcb();
            }
        }

        if(tag = "b")
        {
            cb(uart, sub, term);
        }
        else
        {
            cb(uart, args, term);
        }

    }

    return 0
}

void printCB(io::UART uart, void* args, utils::terminal::Terminal term)
{
    uart.printf("Test Message");
}

void sendCB(io::UART uart, void* args, utils::terminal::Terminal term)
{
    uart.printf(args[0]);
}

void enterCB(io::UART uart, void* sub, utils::terminal::Terminal term)
{
    term.setCurrent(sub);
}

//This print is specifically for MENUITEMS, SUBMENUS, and MENUS
void print(io:UART uart, void* item)
{
    uart.printf(item.toStr());
}

//TERMINAL specific print function
void printTerm(io:UART uart, core::utils:terminal::Terminal term)
{
    uart.printf(term.getMenu().toStr());
}