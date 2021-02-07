/**
 * This example is the hello world application of the EVT common software.
 * The application creates a UART connection which then repeatedly prints out
 * "hello world" at 9600 baud.
 */
#include <iostream>

#include <EVT/utils/bits.hpp>


int main()
{
    std::cout << +getHighByte(1000) << std::endl;
    
    return 0;
}
