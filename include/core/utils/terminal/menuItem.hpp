#ifndef EVT_TERM_MENUITEM
#define EVT_TERM_MENUITEM
#include <core/io/UART.hpp>

//macro for max initial item count of submenus

using callback_t = void (*)(core::io::UART&, char** inputList, void*);

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
            MenuItem(void* head, void* term, char* option, char* text, callback_t cb, void* ctx = nullptr);

            char* getOption(){return option;}

            char* getText(){return text;}

            callback_t getcb(){return cb;}

            void replace(MenuItem* newItem);

            void* getctx(){return ctx;}

            void printStr(core::io::UART& uart);

            /**
             * checks if 2 items are equivalent
             * true if every attribute is equivalent using ==
             * @param it2 a different menu item to compare to
             */
            bool equals(MenuItem* it2);

        private:
            /**
             * key value for item, this is used to select it in your commands
             */
            char* option;

            /**
             * description/name of item
             */
            char* text;

            /**
             * pointer to callback method for this item
             */
            callback_t cb;
            
            /**
             * context for this item, void* because it is of an abstract type
             */
            void* ctx;

            
            /**
             * submenu or menu this item is in
             */
            void* head;

            /**
             * terminal this is in
             */
            void* term;
    };

    class SubMenu : public MenuItem
    {
        public:
            /**
             * constructor for sub-menu sub-class
             */
            SubMenu(void* head, void* term, char* option, char* text, callback_t cb, void* ctx, MenuItem** items);

            /**
             * unique overridden printStr() method for sub-menus
             */
            void printStr(io::UART& uart);

            /**
             * print method that displays the submenu like a menu, instead of an item
             */
            void printMStr(io::UART& uart);

            /**
             * unique overridden equals() method for sub-menus
             * true if every attribute is equivalent, checks all but items with ==
             * items is checked the same way as menu equivalence
             * @param sub2 the other submenu to compare to
             */
            bool equals(SubMenu* sub2);

            /**
             * returns itemCount
             */
            int getCount(){return itemCount;}

            SubMenu* getHead();

            void setItems(MenuItem** itms);
            
            /**
             * returns a list of all items contained in the submenu
             */
            MenuItem** getItems(){return sitems;}

            void enter(io::UART& uart, char** args);

            void exit(io::UART& uart, char** args);

        private:
            /**
             * key value for item, this is used to select it in your commands
             */
            char* option;

            /**
             * description/name of item
             */
            char* text;

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
            MenuItem** sitems;

            /**
             * submenu or menu this item is in
             */
            void* head;

            /**
             * terminal this is in
             */
            void* term;
    };
}

#endif