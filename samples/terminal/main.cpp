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

//example callback that prints a test message
void printCB(io::UART& uart, char** inputList, void* term)
{
    uart.printf("\n\rTest Message\n\r");
}

//example callback that echos user input(called send because e char is taken by exit)
void sendCB(io::UART& uart, char** inputList, void* term)
{
    uart.printf("\n\r");
    for(int i = 1; i < 10; i ++)
    {
        uart.printf(inputList[i]);
        uart.printf(" ");
    }
}

int main()
{
    //core setup
    core::platform::init();

    //UART setup
    io::UART& uart = io::getUART<io::Pin::UART_TX, io::Pin::UART_RX>(9600);

    //item list end item
    utils::MenuItem nul = utils::MenuItem(nullptr,nullptr,"null","LIST END\n\r", nullptr, nullptr);

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
    utils::MenuItem mainPrint = utils::MenuItem(nullptr,t,"p","print test\n\r",printCB,nullptr);
    utils::MenuItem mainSend = utils::MenuItem(nullptr,t,"s","echo\n\r",sendCB,nullptr);
    //submenu cb is exit cb struct, ctx is void* to enter function cb struct. these are NOT responsible for moving menus, just for operations done duirng a menu move
    utils::SubMenu sub = utils::SubMenu(nullptr,t,"sb","sub menu\n\r",nullptr,nullptr,items);
    void* s = &sub;
    utils::MenuItem subPrint = utils::MenuItem(s,t,"p","print test\n\r",printCB,nullptr);
    utils::MenuItem subSend = utils::MenuItem(s,t,"s","echo\n\r",sendCB,nullptr);

    //create submenu
    int c = menu.getCount();
    utils::MenuItem* subs[c];
    subs[0] = &subPrint;
    subs[1] = &subSend;
    subs[2] = &nul;
    sub.setItems(subs);

    //add main menu items to main menu
    items[0] = &mainPrint;
    items[1] = &mainSend;
    items[2] = &sub;
    items[3] = &nul;

    menu.newItems(items);

    //printTerm is how you display the current state of the terminal
    
    uart.printf("Usage: flagChar argument argument | max 9 arguments\n\r");

    //forever while loop to constantly wait for user input
    char* inputList[10];
    char* tag;
    while(true)
    {
        term.printTerm(uart);
        
        term.recieve(inputList);

        tag = inputList[0];
        //void* args[11];
        // for(int i = 0; i < 10; i ++)
        // {
        //     args[i] = inputList[i];
        // }
        //args[10] = &term;

        //term.process(tag, args);
        
        term.process(tag,inputList);
    }


    return 0;
}