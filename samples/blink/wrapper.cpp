#include <cstdio>
#include <stddef.h>

namespace std
{
    extern "C" void _exit(int returnCode)
    {
        while(1)  {}
    }
    
    extern "C" int _kill(int pid, int signal)
    {
        return 0;
    }

    extern "C" int _getpid()
    {
        return 0;
    }
}
