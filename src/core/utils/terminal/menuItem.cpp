#include <core/utils/terminal/menuItem.hpp>
#include <string>

namespace utils
{
    class MenuItem
    {
            utils::MenuItem::MenuItem(std::string option, std::string text, void* cb, void* ctx) : ctx(ctx), cb(cb), text(text), option(option)
            {

            }

            std::string toStr()
            {
                std::string string = option ;
                string += '|';
                string += text;
                out += "\n";
                return string;
            }

            std::string getOption()
            {
                return option;
            }
            std::string getText()
            {
                return text;
            }
            void* getcb()
            {
                return cb;
            }
            void* getctx()
            {
                return ctx;
            }

            bool equals(MenuItem it)
            {
                std::string option2 = it.getOption();
                std::string text2 = it.getText();
                void* cb2 = it.getcb();
                void* ctx2 = it.getctx();
                if(option != option2 || text != text2 || cb != cb2 || ctx != ctx2)
                {
                    return false;
                }
                return true;
            }
    }
}