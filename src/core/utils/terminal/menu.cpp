#include <core/utils/terminal/menuItem.hpp>
#include <core/utils/terminal/menu.hpp>
#include <core/io/UART.hpp>

namespace core::utils
{
    Menu::Menu(MenuItem** items) : items(items)
    {
        //constructor
    }

    void Menu::printStr(core::io::UART& uart) 
    {
        for(int c = 0; c < itemCount; c ++)
        {
            if(items[c]->getOption() == "null")
            {
                 return;
            }
            items[c]->printStr(uart);
        }
    }

    void Menu::addItem(utils::MenuItem* item)
    {
        int c =0;
        for(int i = 0; i < itemCount; i ++)
        {
            if (items[i] == nullptr)
            {
                c = i+1;
                break;
            }
        }
        if(c >= itemCount)
        {
            return;
        }
        items[c-1] = item;
    }

    void Menu::newItems(utils::MenuItem** itms)
    {
        for(int i = 0; i < itemCount; i ++)
        {
            items[i] = itms[i];
        }
    }

    void Menu::delItem(int index)
    {
        items[index] = nullptr;
    }

    void Menu::replace(Menu m)
    {
        utils::MenuItem** i2 = m.getItems();
        for(int i = 0; i < itemCount; i ++)
        {
            items[i] = i2[i];
        }
    }

    bool Menu::equals(Menu* mnu2) 
    {
        MenuItem** items2 = mnu2->getItems();

        for (int i = 0; i < itemCount; i ++)
        {
            if(!(items[i]->equals(items2[i])))
            {
                return false;
            }
        }
        return true;
    }
}