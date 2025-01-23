#ifndef EVT_TERM_MENUITEM
#define EVT_TERM_MENUITEM
#include <core/io/UART.hpp>
#include <string>

//macro for max initial item count of submenus

using callback_t = void (*)(core::io::UART&, void*);

namespace core::utils
{
    class MenuItem
    {
        public:
            /**
             * constructor for menu item object
             * takes a string for the option key, a string for the description text, 
             * a void pointer to the items callback method, and a void pointer to the items context
             * @param option a string representing the items "key", the char/string used to select it
             * @param text a short text description/name of an item
             * @param cb a void pointer to this items callback method
             * @param ctx a void pointer to any context information for this menu(if none provided, is NULL)
             */
            MenuItem(std::string option, std::string text, callback_t cb, void* ctx = nullptr);

            std::string getOption(){return option;}

            std::string getText(){return text;}

            callback_t getcb(){return cb;}

            void* getctx(){return ctx;}

            std::string toStr(MenuItem it);

            /**
             * checks if 2 items are equivalent
             * true if every attribute is equivalent using ==
             * @param it a different menu item to compare to
             */
            bool equals(MenuItem it);

        protected:
            /**
             * key value for item, this is used to select it in your commands
             */
            std::string option;

            /**
             * description/name of item
             */
            std::string text;

            /**
             * pointer to callback method for this item
             */
            callback_t cb;
            
            /**
             * context for this item, void* because it is of an abstract type
             */
            void* ctx;
    };

    class SubMenu : public MenuItem
    {
        public:
            /**
             * constructor for sub-menu sub-class
             */
            SubMenu(std::string option, std::string text, callback_t cb, void* ctx, MenuItem* items);

            /**
             * unique overridden toStr() method for sub-menus
             */
            std::string toStr(SubMenu sub);

            /**
             * unique overridden equals() method for sub-menus
             * true if every attribute is equivalent, checks all but items with ==
             * items is checked the same way as menu equivalence
             * @param sub the other submenu to compare to
             */
            bool equals(SubMenu sub);

            /**
             * returns itemCount
             */
            int getCount(){return itemCount;}
            
            /**
             * returns a list of all items contained in the submenu
             */
            MenuItem* getItems(){return items;}

        private:
            /**
             * key value for item, this is used to select it in your commands
             */
            std::string option;

            /**
             * description/name of item
             */
            std::string text;

            /**
             * pointer to callback method for this item
             */
            callback_t cb;
            
            /**
             * context for this item, void* because it is of an abstract type
             */
            void* ctx;
            /**
             * the total number of items that can be contained in any sub-menu
             */
            int itemCount = 10;

            /**
             * list of all items inside of the sub-menu
             */
            MenuItem* items;
    };
}

#endif