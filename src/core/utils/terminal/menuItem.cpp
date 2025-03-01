#include <core/utils/terminal/menuItem.hpp>
#include <core/io/UART.hpp>
namespace core::utils
{

    MenuItem::MenuItem(void* head, void* term, char* option, char* text, callback_t cb, void* ctx) : head(head), term(term), option(option), text(text), cb(cb), ctx(ctx) 
    {
        
    }

    void MenuItem::printStr(core::io::UART& uart) 
    {
        uart.printf(option); 
        uart.printf("|");
        uart.printf(text);
    }

    void MenuItem::replace(MenuItem* newItem)
    {
        option = newItem->getOption();
        text = newItem->getText();
        cb = newItem->getcb();
        ctx = newItem->getctx();
    }

    bool MenuItem::equals(MenuItem* it2) 
    {
        char* option2 = it2->getOption();
        char* text2 = it2->getText();
        callback_t cb2 = it2->getcb();
        void* ctx2 = it2->getctx();
        if(option != option2 || text != text2 || cb != cb2 || ctx != ctx2)
        {
            return false;
        }
        return true;
    }
}