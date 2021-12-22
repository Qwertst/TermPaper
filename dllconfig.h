#ifndef DLLCONFIG_H
#define DLLCONFIG_H
    #ifdef _WIN32
        #include "windows_dll.h"
    #endif

    #ifdef __linux__
        #include "unix_dll.h"
    #endif
#endif
