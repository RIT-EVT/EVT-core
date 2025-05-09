#ifndef EVT_TERM_STRUCTS
#define EVT_TERM_STRUCTS
#include <core/utils/terminal/terminalManager.hpp>

struct TerminalInterface
{
    //fill with list full of pointers to terminalManager functions to circumvent 
    void** funclist;

    FunctionRunner(void** funclist): funclist(funclist) {}

    void runAll() 
    {
        
    }
};

using callback_t = void (*)(core::io::UART&, char** inputList, void*);


#endif