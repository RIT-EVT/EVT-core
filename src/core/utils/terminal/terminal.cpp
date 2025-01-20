/**
 * 
 */
#include <core/io/UART.hpp>
#include <core/io/pin.hpp>
#include <core/manager.hpp>
#include <core/utils/terminal/terminal.hpp>
#include <core/utils/terminal/Menu.hpp>
#include <core/utils/terminal/MenuItem.hpp>
#include <string>
#include <array>

namespace io = core::io;

namespace core::utils
{
    Terminal::Terminal(io::UART& uart, utils::Menu menu) : menu(menu), uart(uart), current(nullptr)
    {
        uart.printf("Starting Terminal...");
    }

    io::UART& Terminal::getUART()
    {
        return uart;
    }

    utils::Menu Terminal::getMenu()
    {
        return menu;
    }

    utils::SubMenu Terminal::getCurrent()
    {
        return current;
    }

    void Terminal::setCurrent(SubMenu sub)
    {
        current = sub;
    }

    bool Terminal::isMain()
    {
        return m;
    }


    void Terminal::update(std::string message)
    {
        for(int i = 0; i < 5; i ++)
        {
            uart.printf("\n");
        }
        uart.printf(message.c_str());
    }

    std::array<std::string, 10> Terminal::recieve()
    {
        char buffer[100];
        std::string argN[10];
        uart.gets(buffer, 100);
        
        int c = 0;
        char hold[20] = {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};
        h = 0;
        for(int i = 0; i < 100; i ++)
        {
            if(buffer[i] = '\0')
            {
                break;
            }

            if(c >= 10)
            {
                uart.printf("ERROR TOO MANY ARGUMENTS");
                break;
            }

            if(buffer[i] != ' ')
            {
                hold[h] = buffer[i];
                h ++;
            }
            else
            {
                argN[c] = hold;
                hold[20] = {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};
                h = 0;
                c ++;
            }
            
        }

        return argN;

    }
}