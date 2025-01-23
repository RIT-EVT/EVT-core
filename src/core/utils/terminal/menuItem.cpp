#include <core/utils/terminal/menuItem.hpp>
#include <string>

namespace core::utils
{

    MenuItem::MenuItem(std::string option, std::string text, callback_t cb, void* ctx) : option(option), text(text), cb(cb), ctx(ctx) 
    {
        
    }

    std::string toStr(MenuItem it) 
    {
        std::string string = it.getOption();
        string += ('|');
        string += (it.getText());
        string += ("\n");
        return string;
    }

    bool equals(MenuItem it, MenuItem it2) 
    {
        std::string option2 = it2.getOption();
        std::string text2 = it2.getText();
        callback_t cb2 = it2.getcb();
        void* ctx2 = it2.getctx();
        if(it.getOption() != option2 || it.getText() != text2 || it.getcb() != cb2 || it.getctx() != ctx2)
        {
            return false;
        }
        return true;
    }
}