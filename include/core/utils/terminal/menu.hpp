#ifndef EVT_TERM_MENU
#define EVT_TERM_MENU

//macro for max initial item count of a main menu
const int MITEMCOUNT = 10;
#include <core/utils/terminal/menuItem.hpp>
#include <string>

namespace core::utils
{
    class Menu
    {
        public:

            /**
             * Basic constructor takes a list a menu items
             * @param items a list of menu items 
             */
            Menu(MenuItem* items);

            /**
             * creates a string representation of a menu, with each menu item on its own line
             */
            std::string toStr();

            /**
             * returns the list of menu items
             */
            MenuItem* getItems();

            /**
             * returns itemCount
             */
            int getCount();

            /**
             * checks if this menu is equivalent to another menu
             * true if every menu item is equal
             */
            bool equals(Menu mnu);

        protected:

            /**
             * list of all items contained in the menu
             */
            MenuItem* items;

            /**
             * maximum number of items allowed in any menu
             */
            int itemCount = MITEMCOUNT;

    };
}

#endif