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
        //loops through item list, stops when it hits an item with an option string of "null"
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
        //find next empty space
        int c =0;
        for(int i = 0; i < itemCount; i ++)
        {
            if (items[i] == nullptr)
            {
                c = i+1;
                break;
            }
        }
        //make sure next space is within bounds
        if(c >= itemCount)
        {
            return;
        }
        //if in bounds place in list, if not return nothing
        items[c-1] = item;
    }

    void Menu::newItems(utils::MenuItem** itms)
    {
        //replace each item in list with corresponding one in new list
        for(int i = 0; i < itemCount; i ++)
        {
            items[i] = itms[i];
        }
    }

    void Menu::delItem(int index)
    {
        //set desired index to null
        items[index] = nullptr;
        //shift every higher index down
        for(int i = index; i < itemCount-1; i ++)
        {
            items[index] = items[index + 1];
        }\
        //set highest index to null to ensure there is no duplicate final item
        items[itemCount-1] = nullptr;
    }

    void Menu::replace(Menu m)
    {
        //get item list of new menu
        utils::MenuItem** i2 = m.getItems();
        newItems(i2);
    }

    bool Menu::equals(Menu* mnu2) 
    {
        //get second list
        MenuItem** items2 = mnu2->getItems();

        for (int i = 0; i < itemCount; i ++)
        {
            //check if corresponding items are equal, if one fail return false
            if(!(items[i]->equals(items2[i])))
            {
                return false;
            }
        }
        return true;
    }
}