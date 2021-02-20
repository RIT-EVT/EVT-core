#include <cstdio>
#include <stddef.h>

namespace std
{
    extern "C" void _exit(int returnCode)
    {
        while(1)  {}
    }
}
