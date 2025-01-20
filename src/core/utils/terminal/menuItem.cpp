#include <core/utils/terminal/menuItem.hpp>
#include <string>

namespace core::utils
{

    MenuItem::MenuItem(std::string option, std::string text, void* cb, void* ctx) : option(option), text(text), cb(cb), ctx(ctx) 
    {
        
    }

    std::string MenuItem::toStr()
    {
        std::string string = this->option;
        string += ('|');
        string += (this->text);
        string += ("\n");
        return string;
    }

    bool MenuItem::equals(MenuItem it)
    {
        std::string option2 = it.getOption();
        std::string text2 = it.getText();
        void* cb2 = it.getcb();
        void* ctx2 = it.getctx();
        if(this->option != option2 || this->text != text2 || this->cb != cb2 || this->ctx != ctx2)
        {
            return false;
        }
        return true;
    }
}