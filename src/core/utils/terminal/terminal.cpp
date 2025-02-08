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

    char** Terminal::recieve(char** holder)
    {
        char buffer[100];
        char* argN[10];
  
        uart.gets(buffer, 100);


        argN[0] = strtok(buffer, " ");
        for(int i = 1; i < 10; i ++)
        {
            argN[i] = strtok(NULL," ");
        }

        
        // uart.gets(buffer, 100);
        
        // int c = 0;
        // char hold[20];
        // int h = 0;
        // for(int i = 0; i < 100; i ++)
        // {
        //     if(buffer[i] == '\0')
        //     {
        //         break;
        //     }

        //     if(c >= 10)
        //     {
        //         uart.printf("ERROR TOO MANY ARGUMENTS");
        //         break;
        //     }

        //     if(buffer[i] != ' ')
        //     {
        //         hold[h] = buffer[i];
        //         h ++;
        //     }
        //     else
        //     {
        //         argN[c] = hold;
        //         for (int i = 0; i < 20; i++) 
        //         {
        //             hold[i] = '\0';
        //         }
        //         h = 0;
        //         c ++;
        //     }
            
        // }
        holder = argN;
        return holder;

    }
}