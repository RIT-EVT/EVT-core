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
    uart.printf("Test Message");
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
    uart.printf(item.toStr(item).c_str());
}

void print(io::UART& uart, utils::MenuItem item)
{
    uart.printf(item.toStr(item).c_str());
}

void print(io::UART& uart, utils::SubMenu item)
{
    uart.printf(item.toStr(item).c_str());
}

//TERMINAL specific print function
void printTerm(io::UART& uart, core::utils::Terminal term)
{
    utils::Menu* mnu = term.getMenu();
    uart.printf((mnu->toStr(*mnu)).c_str());
}

int main()
{
    //core setup
    core::platform::init();

    //UART setup
    io::UART& uart = io::getUART<io::Pin::UART_TX, io::Pin::UART_RX>(9600);

    //make some items
    utils::MenuItem print = utils::MenuItem("p","Print, takes only key", printCB, nullptr);
    utils::MenuItem send = utils::MenuItem("s","Send, takes key and message", sendCB, nullptr);
    utils::MenuItem items[2] = {print, send};
    utils::SubMenu sub = utils::SubMenu("b", "SubMenu", enterCB, nullptr, items);
    utils::MenuItem items2[3] = {print, send, sub};
    utils::Menu menu = utils::Menu(items2);
    utils::Terminal term = utils::Terminal(uart, &menu);

    printTerm(uart, term);
    
    callback_t cb;
    
    while(true)
    {
        utils::MenuItem* mitems;

        int c = 0;

        if(term.isMain())
        {
            utils::Menu* men = term.getMenu();
            mitems = men->getItems();
            c = men->getCount();
            term.update(men->toStr(*men), uart);
        }
        else
        {
            utils::SubMenu* sub = term.getCurrent();
            mitems = sub->getItems();
            c = sub->getCount();
            term.update(sub->toStr(*sub), uart);
        }
        
        std::array<std::string, 10> inputList = term.recieve(uart);
        std::string tag = inputList[0];
        std::string args[10];
        for (int i = 1; i < 10; i++) 
        {
            args[i - 1] = inputList[i];
        }

        //void (*cb)(io::UART, void*, utils::Terminal);
        

        for(int i = 0; i < c; i ++)
        {
            if(tag == mitems[i].getOption())
            {
                cb = mitems[i].getcb();
            }
        }

        if(tag == "b")
        {
            void* argsArray[2] = { static_cast<void*>(&sub), static_cast<void*>(&term) };
            cb(uart, argsArray);
        }
        else
        {
            cb(uart, args);
        }

    }

    return 0;
}