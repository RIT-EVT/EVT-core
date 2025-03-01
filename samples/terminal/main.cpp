#include <core/utils/terminal/terminal.hpp>
#include <core/utils/terminal/menu.hpp>
#include <core/utils/terminal/menuItem.hpp>
#include <core/io/UART.hpp>
#include <core/io/pin.hpp>
#include <core/manager.hpp>
#include <cstring>
#include <array>

namespace io   = core::io;
namespace utils  = core::utils;

enum dataType
{
    CHAR,
    ITEM,
    TERMINAL
};

//example callback that prints a test message
void printCB(io::UART& uart, void** args)
{
    uart.printf("\n\rTest Message\n\r");
}

//example callback that echos user input(called send because e char is taken by exit)
void sendCB(io::UART& uart, void** args)
{
    uart.printf("\n\r");
    for(int i = 1; i < 10; i ++)
    {
        if(args[i] == nullptr)
        {
            i = 10;
            continue;
        }
        uart.printf((char*)args[i]);
        uart.printf(" ");
    }
}

//This print is specifically for MENUITEMS, SUBMENUS, and MENUS
void print(io::UART& uart, utils::Menu item)
{
    item.printStr(uart);
}

void print(io::UART& uart, utils::MenuItem item)
{
    item.printStr(uart);
}

void print(io::UART& uart, utils::SubMenu item)
{
    item.printStr(uart);
}

int main()
{
    //core setup
    core::platform::init();

    //UART setup
    io::UART& uart = io::getUART<io::Pin::UART_TX, io::Pin::UART_RX>(9600);

    //item list end item
    utils::MenuItem nul = utils::MenuItem(nullptr,nullptr,"null","LIST END", nullptr, nullptr);

    //make some items
    utils::MenuItem* items[10];
    for(int i = 0; i < 10; i ++)
    {
        items[i] = nullptr;
    }
    
    utils::Menu menu = utils::Menu(items);
    utils::Terminal term = utils::Terminal(uart, &menu);
    void* m = &menu;
    void* t = &term;
    //a nullptr in the head parameter means it is part of the main menu
    utils::MenuItem mainPrint = utils::MenuItem(nullptr,t,"p","print test",printCB,nullptr);
    utils::MenuItem mainSend = utils::MenuItem(nullptr,t,"s","echo",sendCB,nullptr);
    //submenu cb is exit cb struct, ctx is void* to enter function cb struct. these are NOT responsible for moving menus, just for operations done duirng a menu move
    utils::SubMenu sub = utils::SubMenu(nullptr,t,"sb","sub menu",nullptr,nullptr,items);
    void* s = &sub;
    utils::MenuItem subPrint = utils::MenuItem(s,t,"p","print test",printCB,nullptr);
    utils::MenuItem subSend = utils::MenuItem(s,t,"s","echo",sendCB,nullptr);

    //create submenu
    int c = menu.getCount();
    utils::MenuItem* subs[c];
    subs[0] = &subPrint;
    subs[1] = &subSend;
    subs[2] = &nul;
    sub.setItems(subs);

    //add main menu items to main menu
    menu.addItem(&mainPrint);
    menu.addItem(&mainSend);
    menu.addItem(&sub);
    menu.addItem(&nul);

    term.printTerm(uart);
    uart.printf("Usage: flagChar argument argument | max 9 arguments");
    callback_t cb;
    while(true)
    {
        char* inputList[10];
        term.recieve(inputList);
        char* tag = inputList[0];
        void* args[11];
        for(int i = 0; i < c; i ++)
        {
            if(inputList[i] == "\0")
            {
                args[i-1] = nullptr;
                break;
            }
            args[i-1] = inputList[i];
        }
        args[10] = &term;

        utils::MenuItem* chosen = &nul;
        
        term.process(chosen, tag);

        cb = chosen->getcb();

        cb(uart, args);

        term.printTerm(uart);
    }

    return 0;
}