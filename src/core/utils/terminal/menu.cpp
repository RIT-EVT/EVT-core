#include <core/utils/terminal/menuItem.hpp>
#include <core/utils/terminal/menu.hpp>
#include <core/io/UART.hpp>

namespace core::utils
{
    Menu::Menu(MenuItem* items) : items(items)
    {
        
    }

    void Menu::printStr(core::io::UART& uart) 
    {
        for(int c = 0; c < itemCount; c ++)
        {
            if(items[c].getOption() == "null")
            {
                return;
            }
            items[c].printStr(uart);
        }
    }

    bool Menu::equals(Menu mnu2) 
    {
        MenuItem* items2 = mnu2.getItems();

        for (int i = 0; i < itemCount; i ++)
        {
            if(!(items[i].equals(items2[i])))
            {
                return false;
            }
        }
        return true;
    }
}