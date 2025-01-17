#include <core/utils/terminal/menuItem.hpp>
#include <core/utils/terminal/menu.hpp>
#include <string>

namespace utils
{
    class Menu
    {
            utils::Menu::Menu(core::utils::MenuItem item[ITEMCOUNT]) : items(items)
            {

            }

            std::string toStr()
            {
                std::string out = "";

                for(int i = 0; i < getCount(); i ++)
                {
                    out += items[0].toStr() 
                    out += "\n";
                }

                return out;
            }

            int getCount()
            {
                return itemCount;
            }

            core::utils::MenuItem[itemCount] getItems()
            {
                return items;
            }

            bool equals(Menu mnu) : mnu(mnu)
            {
                core::utils::MenuItem items2[itemCount] = mnu.getItems();
                for (i = 0; i < itemCount; i ++)
                {
                    if(!(items[i].equals(items2[i])))
                    {
                        return false;
                    }
                }
                return true;
            }
    }
}