#include <menuItem.hpp>
#include <subMenu.hpp>



namespace core:utils
{
    class Menu
    {
        public:

            Menu(menuItem[itemCount] items);

        private:

            menuItem[itemCount] items;

            int itemCount = 10;

    }
}