
#include <menuItem.hpp>
#include <string>


namespace core:utils
{
    class SubMenu : public MenuItem
    {
            SubMenu::SubMenu(std::string option, std::string text, void* cb, void* ctx, menuItem[itemCount] items)
            {

            }

            std::string toStr()
            {
                //create string for line showing this sub-menu
                std::string out = option 
                out += "|"
                out += text;
                out += "\n"
                return out;
            }

            MenuItem[itemCount] getItems()
            {
                return items;
            }

            bool equals(MenuItem sub) : sub(sub)
            {
                //check equivalence of attributes besides items 
                //this section is identical to menuItem
                std::string option2 = it.getOption();
                std::string text2 = it.getText();
                void* cb2 = it.getcb();
                void* ctx2 = it.getctx();
                MenuItem items2[itemCount] = it.getItems();
                if(option != option2 || text != text2 || cb != cb2 || ctx != ctx2)
                {
                    return false;
                }

                //Check items equivalence
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