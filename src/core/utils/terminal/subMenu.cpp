#include <core/utils/terminal/menuItem.hpp>
#include <core/io/UART.hpp>


namespace core::utils
{
    SubMenu::SubMenu(void* parent, void* term, char* option, char* text, callback_t cb, void* ctx, MenuItem** sitems) : parent(parent), term(term), option(option), text(text), cb(cb), ctx(ctx), sitems(sitems), MenuItem(parent, term, option, text, cb, ctx)
    {
        //constructor
    }

    void SubMenu::printStr(core::io::UART& uart) 
    {
        //this is exclusively for displaying the submenu inside of a menu list
        // option | text
        uart.printf(option); 
        uart.printf("|");
        uart.printf(text);
    }

    void SubMenu::printMStr(core::io::UART& uart)
    {
        //used for printing this as the current menu state
        //prints all items in list until it hits one with option string "null"
        for(int c = 0; c < itemCount; c ++)
        {
            if(sitems[c]->getOption() == "null")
            {
                return;
            }
            sitems[c]->printStr(uart);
        }
    }

    void SubMenu::setItems(MenuItem** itms)
    {
        //replaces corresponding items in sitems with itms
        //used to replace item list, or certain items by inputing an altered item list
        for(int i = 0; i < itemCount; i++)
        {
            sitems[i] = itms[i];
        }
    }

    bool SubMenu::equals(SubMenu* sub2) 
    {
        //check equivalence of attributes besides items 
        //this section is identical to menuItem
        char* option2 = sub2->getOption();
        char* text2 = sub2->getText();
        callback_t cb2 = sub2->getcb();
        void* ctx2 = sub2->getctx();
        MenuItem** items2 = sub2->getItems();
        if(option != option2 || text != text2 || cb != cb2 || ctx != ctx2)
        {
            return false;
        }

        //Check items equivalence;
        for (int i = 0; i < itemCount; i ++)
        {
            if(!(sitems[i]->equals(items2[i])))
            {
                return false;
            }
        }
        return true;
    }

    void SubMenu::enter(io::UART& uart, char** args)
    {
        cb(uart,args,term);
    }

    void SubMenu::exit(io::UART& uart, char** args)
    {
        //if ctx has value use it as a callback
        if(ctx)
        {
            callback_t cb = (callback_t)ctx;
            cb(uart, args, term);
        }
    }
}