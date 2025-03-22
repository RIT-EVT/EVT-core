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
        m = true;
    }

    void Terminal::update(char* message, utils::Menu m = nullptr)
    {
        menu->replace(m);
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

    //TERMINAL specific print function
    void Terminal::printTerm(io::UART& uart)
    {
        uart.printf("\n\r");
        uart.printf("Terminal:\n\r");
        if(m)
        {
            menu->printStr(uart);
        }
        else
        {
            current->printMStr(uart);
            uart.printf("e|exit\n\r");
        }
    }

    void Terminal::process(char* tag, char** args)
    {
        
        utils::MenuItem* holder;
        if(m)
        {
            utils::MenuItem** subitemsM = menu->getItems();

            for(int i = 0; i < menu->getCount(); i ++)
            {
                char* op = subitemsM[i]->getOption();

                if(strcmp(op, "null") == 0)
                {
                    holder = nullptr;
                    break;
                }
                else if(strcmp(tag,op) == 0)
                {
                    holder = (subitemsM[i]);
                    break;
                }
            }
        }
        else
        {
            utils::MenuItem** subitemsC = current->getItems();

            for(int i = 0; i < menu->getCount(); i ++)
            {
                char* op = subitemsC[i]->getOption();
                if(strcmp(op, "null") == 0)
                {
                    holder = nullptr;
                    break;
                }
                if(strcmp(tag,op) == 0)
                {
                    holder = (subitemsC[i]);
                    break;
                }
            }
        }

        if(strcmp(tag, "e") == 0)
        {
            current->exit(uart,nullptr);
            void* h = current->getHead();
            if(h)
            {
                current->replace((utils::SubMenu*)h);
            }

            utils::MenuItem out = utils::MenuItem(nullptr,nullptr,"EXIT","EXIT",nullptr,nullptr);
            holder->replace(&out);
        }

        if(holder == nullptr || strcmp(holder->getOption(),"EXIT") == 0)
        {
            return;
        }

        callback_t cb = holder->getcb();
        
        cb(uart, args, (void*)this);
    }
}