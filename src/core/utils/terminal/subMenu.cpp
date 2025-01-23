#include <core/utils/terminal/menuItem.hpp>
#include <string>


namespace core::utils
{
    SubMenu::SubMenu(std::string option, std::string text, callback_t cb, void* ctx, MenuItem* items) : MenuItem(option, text, cb, ctx)
    {
        this->items = items;
    }

    std::string toStr(SubMenu sub) 
    {
        //create string for line showing this sub-menu
        std::string out = sub.getOption();
        out += ("|");
        out += (sub.getText());
        out += ("\n");
        return out;
    }

    bool equals(SubMenu sub, SubMenu sub2) 
    {
        //check equivalence of attributes besides items 
        //this section is identical to menuItem
        std::string option2 = sub2.getOption();
        std::string text2 = sub2.getText();
        callback_t cb2 = sub2.getcb();
        void* ctx2 = sub2.getctx();
        MenuItem* items2 = sub2.getItems();
        if(sub.getOption() != option2 || sub.getText() != text2 || sub.getcb() != cb2 || sub.getctx() != ctx2)
        {
            return false;
        }

        //Check items equivalence
        MenuItem* items = sub.getItems();
        for (int i = 0; i < sub.getCount(); i ++)
        {
            if(!(items[i].equals(items2[i])))
            {
                return false;
            }
        }
        return true;
    }
}