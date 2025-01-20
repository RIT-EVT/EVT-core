#include <core/utils/terminal/menuItem.hpp>
#include <string>


namespace core::utils
{
    SubMenu::SubMenu(std::string option, std::string text, void* cb, void* ctx, MenuItem* items) : MenuItem(option, text, cb, ctx)
    {
        this->items = items;
    }

    std::string SubMenu::toStr()
    {
        //create string for line showing this sub-menu
        std::string out = this->option;
        out += ("|");
        out += (this->text);
        out += ("\n");
        return out;
    }

    MenuItem* SubMenu::getItems()
    {
        return items;
    }

    int SubMenu::getCount()
    {
        return itemCount;
    }

    bool SubMenu::equals(SubMenu sub)
    {
        //check equivalence of attributes besides items 
        //this section is identical to menuItem
        std::string option2 = sub.getOption();
        std::string text2 = sub.getText();
        void* cb2 = sub.getcb();
        void* ctx2 = sub.getctx();
        MenuItem* items2 = sub.getItems();
        if(this->option != option2 || this->text != text2 || this->cb != cb2 || this->ctx != ctx2)
        {
            return false;
        }

        //Check items equivalence
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