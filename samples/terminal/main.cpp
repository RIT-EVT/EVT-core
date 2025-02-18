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

void printCB(io::UART& uart, void** args)
{
    uart.printf("\n\rTest Message\n\r");
}

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

void enterCB(io::UART& uart, void** args)
{
    utils::Terminal* term = (utils::Terminal*)(args[10]);

    utils::MenuItem** items = term->getMenu()->getItems();

    utils::MenuItem* chosen;
    char* tag = (char*)args[0];
    for(int i = 0; i < 10; i ++)
            {
                char* op = items[i]->getOption();
                if(strcmp(op, "null") == 0)
                {
                    break;
                }
                if(strcmp(tag,op) == 0)
                {
                    chosen = (items[i]);
                    break;
                }
            }
    term->setCurrent((utils::SubMenu*)chosen);
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

//TERMINAL specific print function
void printTerm(io::UART& uart, core::utils::Terminal term)
{
    uart.printf("\n\r");
    if(term.isMain())
    {
        utils::Menu* mnu = term.getMenu();
        mnu->printStr(uart);
    }
    else
    {
        term.getCurrent()->printMStr(uart);
    }
}

int main()
{
    //core setup
    core::platform::init();

    //UART setup
    io::UART& uart = io::getUART<io::Pin::UART_TX, io::Pin::UART_RX>(9600);

    //item list end item
    utils::MenuItem nul = utils::MenuItem("null","LIST END", nullptr, nullptr);

    //make some items
    utils::MenuItem print = utils::MenuItem("p","Print, takes only key\n\r", printCB, nullptr);
    utils::MenuItem send = utils::MenuItem("s","Send, takes key and message\n\r", sendCB, nullptr);
    utils::MenuItem* items[3] = {&print, &send, &nul};
    utils::SubMenu sub = utils::SubMenu("b", "SubMenu\n\r", enterCB, nullptr, items);
    utils::MenuItem* items2[4] = {&print, &send, &sub, &nul};
    utils::Menu menu = utils::Menu(items2);
    utils::Terminal term = utils::Terminal(uart, &menu);

    printTerm(uart, term);
    
    callback_t cb;

    while(true)
    {
        utils::MenuItem* mitems;

        int c = 0;

        char* inputList[10];
        term.recieve(inputList);
        char* tag = inputList[0];
        void* args[11];
        for(int i = 0; i < 10; i ++)
        {
            if(inputList[i] == "\0")
            {
                args[i-1] = nullptr;
                break;
            }
            args[i-1] = inputList[i];
        }
        args[10] = &term;

        bool m = term.isMain();

        utils::Menu* men = term.getMenu();
        
        utils::MenuItem** subitemsM = men->getItems();
        
        utils::SubMenu* cur = term.getCurrent();
        
        utils::MenuItem** subitemsC;

        
        if(!m)
        { 
            subitemsC = cur->getItems();
        }

        utils::MenuItem* chosen = &nul;
        if(m)
        {
            for(int i = 0; i < 10; i ++)
            {
                char* op = subitemsM[i]->getOption();
                if(strcmp(op, "null") == 0)
                {
                    break;
                }
                if(strcmp(tag,op) == 0)
                {
                    chosen = (subitemsM[i]);
                    break;
                }
            }
        }
        else
        {
            for(int i = 0; i < 10; i ++)
            {
                char* op = subitemsC[i]->getOption();
                if(strcmp(op, "null") == 0)
                {
                    break;
                }
                if(strcmp(tag,op) == 0)
                {
                    chosen = (subitemsC[i]);
                    break;
                }
            }
        }
        cb = chosen->getcb();

        cb(uart, args);

        printTerm(uart, term);
    }

    return 0;
}