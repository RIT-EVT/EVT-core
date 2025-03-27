#ifndef EVT_TERM_MENU
#define EVT_TERM_MENU

//macro for max initial item count of a main menu
#include <core/utils/terminal/menuItem.hpp>


namespace core::utils
{
    class Menu
    {
        public:

            /**
             * Basic constructor takes a list a menu items
             * @param items a list of menu items 
             */
            Menu(MenuItem** items);

            /**
             * creates a string representation of a menu, with each menu item on its own line
             */
            void printStr(io::UART& uart);

            /**
             * returns number of items in menu
             */
            int getCount(){return itemCount;}

            /**
             * replaces this instance of a menu with another menu
             * @param m the menu you want to replace it with
             */
            void replace(Menu m);

            /**
             * returns list of items in menu
             */
            MenuItem** getItems() {return items;}

            /**
             * adds an item to the menu
             * @param item the item to add
             */
            void addItem(utils::MenuItem* item);

            /**
             * replaces the current list of items with a new one
             * @param itms the list to replace current with
             */
            void newItems(utils::MenuItem** itms);

            /**
             * delete an item from the list
             * @param index the index of the item in the list(top of list when printing is 0)
             */
            void delItem(int index);


            /**
             * checks if this menu is equivalent to another menu
             * true if every menu item is equal
             * @param mnu2 the menu to compare to
             */
            bool equals(Menu* mnu2);

        private:

            /**
             * list of all items contained in the menu
             */
             MenuItem** items;

            /**
             * maximum number of items allowed in any menu
             */
             int itemCount = 10;

    };
}

#endif