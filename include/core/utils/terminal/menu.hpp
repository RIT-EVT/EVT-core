#ifndef EVT_TERM_MENU
#define EVT_TERM_MENU

// macro for max initial item count of a main menu
#include <core/utils/terminal/menuItem.hpp>
#include <core/utils/terminal/terminal.hpp>

namespace core::utils {

    
class Menu : public MenuItem{
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
    SubMenu(Menu* parent, void* term, char* option, char* text, callback_t cb, void* ctx, MenuItem** items);

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
     * if nullptr the current node is the head
     */
    Menu* parent;

    /**
     * terminal this is in
     */
    void* term;
};
} // namespace core::utils

#endif