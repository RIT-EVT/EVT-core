/**
 * 
 */
#include <core/io/UART.hpp>
#include <core/io/pin.hpp>
#include <core/utils/terminal/terminal.hpp>
#include <core/utils/terminal/Menu.hpp>
#include <core/utils/terminal/MenuItem.hpp>
#include <cstring>

namespace io = core::io;

namespace core::utils
{
    Terminal::Terminal(io::UART& uart, utils::Menu* menu) : menu(menu), uart(uart)
    {
        uart.printf("\n\rStarting Terminal...\n\r");
    }

    void Terminal::update(char* message)
    {
        for(int i = 0; i < 5; i ++)
        {
            uart.printf("\n\r");
        }
        uart.printf(message);
    }

    void Terminal::setCurrent(SubMenu* sub)
    {
        current = sub;
        m = false;
    }

    void Terminal::recieve(char* holder[10])
    {
        char buffer[99];
        char* argN[10];
  
        uart.gets(buffer, 99);

        argN[0] = strtok(buffer, " ");
        for(int i = 1; i < 10; i ++)
        {
            argN[i] = strtok(NULL, " ");
        }

        for(int i = 0; i < 10; i++)
        {
            holder[i] = argN[i];
        }
    }
}