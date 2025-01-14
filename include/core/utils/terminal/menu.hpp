#include <menuItem.hpp>
#include <string>

namespace core:utils
{
    class Menu
    {
        public:

            /**
             * Basic constructor takes a list a menu items
             * @param items a list of menu items 
             */
            Menu(MenuItem[itemCount] items);

            /**
             * creates a string representation of a menu, with each menu item on its own line
             */
            std::string toStr();

            /**
             * returns the list of menu items
             */
            MenuItem[itemCount] getItems();

            /**
             * returns itemCount
             */
            int getCount();

            /**
             * checks if this menu is equivalent to another menu
             * true if every menu item is equal
             */
            bool equals(Menu mnu)

        private:

            /**
             * list of all items contained in the menu
             */
            MenuItem[itemCount] items;

            /**
             * maximum number of items allowed in any menu
             */
            int itemCount = 10;

    }
}