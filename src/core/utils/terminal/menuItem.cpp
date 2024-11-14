#include <menuItem.hpp>


namespace core:utils
{
    class MenuItem
    {
            MenuItem::MenuItem(char* option, char* text, void* cb, void* ctx) : ctx(ctx), cb(cb), text(text), option(option)
            {

            }
    }
}