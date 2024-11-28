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
    

    utils::Terminal::MenuItem print = MenuItem::MenuItem();
    utils::Terminal::MenuItem send = MenuItem::MenuItem();
    utils::Terminal::SubMenu sub = MenuItem::SubMenu();
    utils::Terminal::Menu menu = Menu::Menu();
    utils::Terminal::Terminal term = Terminal::Terminal();
}


std:string print(utils::Terminal::Terminal term)
{
    io::UART uart = term.getUART();
}