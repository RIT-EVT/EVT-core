#include <core/utils/terminal/menuItem.hpp>
#include <core/io/UART.hpp>


namespace core::utils
{
    SubMenu::SubMenu(void* head, void* term, char* option, char* text, callback_t cb, void* ctx, MenuItem** items) : head(head), term(term), option(option), text(text), cb(cb), ctx(ctx), items(items), MenuItem(head, term, option, text, cb, ctx)//SubMenu(option, text, cb, ctx, items)
    {

    }

    void SubMenu::printStr(core::io::UART& uart) 
    {
        //this is exclusively for displaying the submenu inside of a menu list
        uart.printf(option); 
        uart.printf("|");
        uart.printf(text);
    }

    void SubMenu::printMStr(core::io::UART& uart)
    {
        for(int c = 0; c < itemCount; c ++)
        {
            if(items[c]->getOption() == "null")
            {
                return;
            }
            items[c]->printStr(uart);
        }
        uart.printf("e|exit\n\r");
    }

    SubMenu* SubMenu::getHead()
    {
        if(head)
        {
            return (utils::SubMenu*)head;
        }
        return nullptr;

    }

    void SubMenu::setItems(MenuItem** itms)
    {
        for(int i = 0; i < itemCount; i++)
        {
            if(itms[i] == nullptr)
            {
                break;
            }
            items[i] = itms[i];
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
            if(!(items[i]->equals(items2[i])))
            {
                return false;
            }
        }
        return true;
    }

    void SubMenu::enter(io::UART& uart, void** args)
    {
        if(ctx){
        callback_t cb = (callback_t)ctx;
        cb(uart, args);
        }
    }

    void SubMenu::exit(io::UART& uart, void** args)
    {
        cb(uart, args);
    }
}