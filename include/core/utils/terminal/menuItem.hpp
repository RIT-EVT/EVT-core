#ifndef EVT_TERM_MENUITEM
#define EVT_TERM_MENUITEM
#include <core/io/UART.hpp>
#include <terminal.hpp>

// macro for max initial item count of submenus

// struct used for callback functions, a uart instance to communicate over,
// a list of input strings(your input from the terminal), and a void*
// mostly a placeholder to ease handling void* to a function, when you fill with a function make sure the parameters are
// (UART,char**,void*)

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
} // namespace core::utils

#endif