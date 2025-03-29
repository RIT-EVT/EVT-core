#ifndef EVT_TERM_MENUITEM
#define EVT_TERM_MENUITEM
#include <core/io/UART.hpp>

// macro for max initial item count of submenus

// struct used for callback functions, a uart instance to communicate over,
// a list of input strings(your input from the terminal), and a void*
// mostly a placeholder to ease handling void* to a function, when you fill with a function make sure the parameters are
// (UART,char**,void*)
using callback_t = void (*)(core::io::UART&, char** inputList, void*);

namespace core::utils {
class MenuItem {
public:
    /**
     * constructor for menu item object
     * takes a string for the option key, a string for the description text,
     * a void pointer to the items callback method, and a void pointer to the items context
     * @param parent pointer to parent node
     * @param term pointer to terminal instance this item resides within
     * @param option a string representing the items "key", the char/string used to select it
     * @param text a short text description/name of an item
     * @param cb a void pointer to this items callback method
     * @param ctx a void pointer to any context information for this menu(if none provided, is NULL)
     */
    MenuItem(void* parent, void* term, char* option, char* text, callback_t cb, void* ctx);

    /**
     * option acessor
     */
    char* getOption() {
        return option;
    }

    /**
     * returns parent
     */
    void* getParent() {
        return parent;
    }

    /**
     * text acessor
     */
    char* getText() {
        return text;
    }

    /**
     * callback acessor
     */
    callback_t getcb() {
        return cb;
    }

    /**
     * replace this item with another one
     * @param newItem the new item
     */
    void replace(MenuItem* newItem);

    /**
     * context acessor
     */
    void* getctx() {
        return ctx;
    }

    /**
     * print string representation
     * @param uart the uart instance to print over
     */
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
    void* parent;

    /**
     * terminal this is in
     */
    void* term;
};

class SubMenu : public MenuItem {
public:
    /**
     * constructor for sub-menu sub-class
     * @param parent same as menuitem
     * @param term same as menuitem
     * @param option same as menuitem
     * @param text same as menuitem
     * @param cb exit behavior callback, leave null for nothing
     * @param ctx enterence behavior callback void*(leave null for nothing)
     * @param items list of items in submenu
     */
    SubMenu(void* parent, void* term, char* option, char* text, callback_t cb, void* ctx, MenuItem** items);

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
    int getCount() {
        return itemCount;
    }

    /**
     * returns parent
     */
    void* getParent() {
        return parent;
    }

    /**
     * replaces current item list with provided one
     * @param itms items to replace current list with
     */
    void setItems(MenuItem** itms);

    /**
     * returns a list of all items contained in the submenu
     */
    MenuItem** getItems() {
        return sitems;
    }

    // #
    // # USE THESE TO DO ANY NEEDED SETUP/CLEANUP WHEN ENTERING/EXITING A SUB MENU:
    // #

    /**
     * automatic callback executor for entering
     * custom behavor is stored in ctx void*
     * if cb is empty will do nothing
     * @param uart uart instance to use for cb
     * @param args arguments for cb
     */
    void enter(io::UART& uart, char** args);

    /**
     * automatic callback executor for exiting
     * custom behavor is stored in cb.
     * if ctx is nullptr will do nothing
     * @param uart uart instance to use for cb
     * @param args arguments for cb
     */
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
    void* parent;

    /**
     * terminal this is in
     */
    void* term;
};
} // namespace core::utils

#endif