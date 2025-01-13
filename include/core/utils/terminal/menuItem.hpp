#include <string>

namespace core:utils
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
            MenuItem(std::string option, std::string text, void* cb, void* ctx = nullptr);

            /**
             * returns a strings representation of the menu item
             * this is the key, followed by the text description/name 
             */
            std::string toStr();

            /**
             * returns the option attribute that is the items key as a string
             */
            std::string getOption();
            
            /**
             * returns description text/name as a string
             */
            std::string getText();

            /**
             * returns callback method pointer
             */
            void* getcb();

            /**
             * returns context for this item
             */
            void* getctx();

            /**
             * checks if 2 items are equivalent
             * true if every attribute is equivalent using ==
             * @param it a different menu item to compare to
             */
            bool equals(MenuItem it);

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
            void* cb;
            
            /**
             * context for this item, void* because it is of an abstract type
             */
            void* ctx;
    };

    class SubMenu : MenuItem
    {
        public:
            /**
             * constructor for sub-menu sub-class
             */
            SubMenu(std::string option, std::string text, void* cb, void* ctx, MenuItem[itemCount] items);

            /**
             * unique overridden toStr() method for sub-menus
             */
            std::string toStr();

            /**
             * unique overridden equals() method for sub-menus
             * true if every attribute is equivalent, checks all but items with ==
             * items is checked the same way as menu equivalence
             * @param sub the other submenu to compare to
             */
            bool equals(SubMenu sub);
            MenuItem[itemCOunt] getItems();

        private:
            /**
             * the total number of items that can be contained in any sub-menu
             */
            int itemCount = 10;

            /**
             * list of all items inside of the sub-menu
             */
            MenuItem[10] items;
    };
}