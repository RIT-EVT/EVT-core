#include <core/utils/terminal/menuItem.hpp>
#include <core/utils/terminal/menu.hpp>
#include <string>


namespace core::utils
{
    Menu::Menu(MenuItem* items) : items(items)
    {

    }

    std::string Menu::toStr()
    {
        std::string out = "";
        MenuItem* items = this->items;
        for(int i = 0; i < MITEMCOUNT; i ++)
        {
           out += items[0].toStr();
           out += "\n";
        }

        return out;
    }

    int Menu::getCount()
    {
        return MITEMCOUNT;
    }

    MenuItem* Menu::getItems()
    {
        return this->items;
    }

    bool Menu::equals(Menu mnu)
    {
        MenuItem* items2;
        std::copy(items2[0],items2[MITEMCOUNT-1],(mnu.getItems())[0]);
        for (int i = 0; i < MITEMCOUNT; i ++)
        {
            if(!(items[i].equals(items2[i])))
            {
                return false;
            }
        }
        return true;
    }
}