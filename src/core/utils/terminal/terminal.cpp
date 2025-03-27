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

    bool Terminal::recieve(char* holder[10])
    {
        for(int i = 0; i < 99; i ++)
        {
            buffer[i] = '\0';
        }
        uart.gets(buffer, 99);

        holder[0] = strtok(buffer, " ");
        for(int i = 1; i < 10; i ++)
        {
            holder[i] = strtok(NULL, " ");
        }
        return true;
        // for(int i = 0; i < 10; i++)
        // {
        //     holder[i] = argN[i];
        // }
    }

    //TERMINAL specific print function
    void Terminal::printTerm()
    {
        uart.printf("\n\r");
        uart.printf("Terminal:\n\r");
        uart.printf("is main: ");
        if(m)
        {
            uart.printf("true\n\r");
        }
        else{uart.printf("false\n\r");}
        
        if(m)
        {
            menu->printStr(uart);
        }
        else
        {
            current->printMStr(uart);
            uart.printf("q|QUIT\n\r");
        }
    }

    void Terminal::enterSub(io::UART& uart, char** args, void* term)
    {
        utils::MenuItem** items = menu->getItems();
        char* name = args[0];
        int c = menu->getCount();
        utils::MenuItem* item;
        for(int i = 0; i < c; i ++)
        {
            item = items[i];
            if(strcmp(name, item->getOption()) == 0)
            {
                setCurrent((utils::SubMenu*)item);
                break;
            }
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

        if(strcmp(tag, "q") == 0)
        {
            m = true;
        }

        if(holder == nullptr || strcmp(holder->getOption(),"QUIT") == 0)
        {
            return;
        }

        callback_t cb = holder->getcb();
        
        cb(uart, args, (void*)this);
    }
}