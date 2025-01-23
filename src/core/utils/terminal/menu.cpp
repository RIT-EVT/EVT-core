#include <core/utils/terminal/menuItem.hpp>
#include <core/utils/terminal/menu.hpp>
#include <string>


namespace core::utils
{
    Menu::Menu(MenuItem* items) : items(items)
    {
        
    }

    std::string toStr(Menu mnu) 
    {
        std::string out = "";
        MenuItem* items = mnu.getItems();
        for(int i = 0; i < mnu.getCount(); i ++)
        {
           out += items[0].toStr(items[0]);
           out += "\n";
        }

        return out;
    }

    bool equals(Menu mnu) 
    {
        MenuItem* items2;
        std::copy(items2, items2 + mnu.getCount(), mnu.getItems());

        for (int i = 0; i < mnu.getCount(); i ++)
        {
            if(!(mnu.getItems()[i].equals(items2[i])))
            {
                return false;
            }
        }
        return true;
    }
}