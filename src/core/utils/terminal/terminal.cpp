/**
 * 
 */
#include <core/io/UART.hpp>
#include <core/io/pin.hpp>
#include <core/manager.hpp>
#include <core/utils/terminal/terminal.hpp>
#include <string>

namespace utils
{
    class Terminal
    {
        Terminal::Terminal(io::Uart& uart, utils::Terminal::Menu menu) : menu(menu), uart(uart)
        {
            uart.printf("Starting Terminal...");
        }

        io::UART& getUART()
        {
            return uart;
        }

        utils::Menu getMenu()
        {
            return menu;
        }

        void update(std::string message)
        {
            for(int i = 0; i < 5; i ++)
            {
                uart.printf("\n");
            }
            uart.printf(message);
        }

        std::string[10] recieve()
        {
            char buffer[100];
            std::string argN[10];
            uart.gets(buffer, 100);
            
            int c = 0;
            char hold[20] = {'','','','','','','','','','','','','','','','','','','',''};
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
                    hold[20] = {'','','','','','','','','','','','','','','','','','','',''};
                    h = 0;
                    c ++;
                }
                
            }

            return argN;

        }
    }
}