#include <menuItem.hpp>
#include <subMenu.hpp>
#include <string>

namespace core:utils
{
    class Menu
    {
            Menu::Menu(MenuItem[itemCount] items) : items(items)
            {

            }

            std::string toStr()
            {
                std::string out = "";

                for(int i = 0; i < itemCount; i ++)
                {
                    out += MenuItem[0].toStr() 
                    out += "\n";
                }

                return out;
            }

            MenuItem[itemCount] getItems()
            {
                return items;
            }

            bool equals(Menu mnu) : mnu(mnu)
            {
                MenuItem items2[itemCount] = mnu.getItems();
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