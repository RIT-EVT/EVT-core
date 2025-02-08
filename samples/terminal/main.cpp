#include <core/utils/terminal/terminal.hpp>
#include <core/utils/terminal/menu.hpp>
#include <core/utils/terminal/menuItem.hpp>
#include <core/io/UART.hpp>
#include <core/io/pin.hpp>
#include <core/manager.hpp>
#include <string>
#include <array>

namespace io   = core::io;
namespace utils  = core::utils;

void printCB(io::UART& uart, void* args)
{
    uart.printf("Test Message\n\r");
}

void sendCB(io::UART& uart, void* args)
{
    void* argsArray = static_cast<void*>(args);
    char* message = static_cast<char*>(argsArray);
    uart.printf(message);
}

void enterCB(io::UART& uart, void* args)
{
    // Cast the void pointer to an array of void pointers
    void** argsArray = static_cast<void**>(args);
    
    // Cast the first element of the array to SubMenu*
    utils::SubMenu* sub = static_cast<utils::SubMenu*>(argsArray[0]);
    
    // Cast the second element of the array to Terminal*
    utils::Terminal* term = static_cast<utils::Terminal*>(argsArray[1]);

    // Set the current submenu
    term->setCurrent(sub);
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
    utils::MenuItem items[3] = {print, send, nul};
    utils::SubMenu sub = utils::SubMenu("b", "SubMenu\n\r", enterCB, nullptr, items);
    utils::MenuItem items2[4] = {print, send, sub, nul};
    utils::Menu menu = utils::Menu(items2);
    utils::Terminal term = utils::Terminal(uart, &menu);

    printTerm(uart, term);
    
    callback_t cb;

    while(true)
    {
        utils::MenuItem* mitems;

        int c = 0;

        char** inputList;
        term.recieve(inputList);
        char* item = inputList[0];

        bool m = term.isMain();
        utils::Menu* men = term.getMenu();
        
        utils::MenuItem* subitemsM = men->getItems();
        
        utils::SubMenu* cur = term.getCurrent();
        
        utils::MenuItem* subitemsC;

        if(!m)
        { 
            subitemsC = cur->getItems();
        }

        utils::MenuItem* chosen;
            
        if(m)
        {
            for(int i = 0; i < 10; i ++)
            {
                if(item == subitemsM[i].getOption())
                {
                    *chosen = subitemsM[i];
                    break;
                }
            }
        }
        else
        {
            for(int i = 0; i < 10; i ++)
            {
                if(item == subitemsC[i].getOption())
                {
                    *chosen = subitemsC[i];
                    break;
                }
            }
        }

        uart.printf("\n\rend\n\r");
    }

    return 0;
}