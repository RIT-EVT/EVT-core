#include <core/utils/terminal/terminal.hpp>
#include <core/utils/terminal/menu.hpp>
#include <core/utils/terminal/menuItem.hpp>
#include <core/io/UART.hpp>
#include <core/io/pin.hpp>
#include <core/manager.hpp>
#include <string>

namespace io   = core::io;
namespace utils  = core::dev::utils;

int main()
{
    //core setup
    core::platform::init();

    //UART setup
    io::UART& uart = io::getUART<io::Pin::UART_TX, io::Pin::UART_RX>(9600);

    //make some items
    utils::Terminal::MenuItem print = MenuItem::MenuItem();
    utils::Terminal::MenuItem send = MenuItem::MenuItem();
    utils::Terminal::SubMenu sub = MenuItem::SubMenu();
    utils::Terminal::Menu menu = Menu::Menu();
    utils::Terminal::Terminal term = Terminal::Terminal();

    printTerm(uart, term);
    

    return 0
}

//This print is specifically for MENUITEMS, SUBMENUS, and MENUS
void print(io:UART uart, void* item)
{
    uart.printf(item.toStr());
}


//TERMINAL specific print function
void printTerm(io:UART uart, utils:Terminal::Terminal term)
{
    uart.printf(term.getMenu().toStr());
}