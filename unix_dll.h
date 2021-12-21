#ifndef UNIX_DLL_H
#define UNIX_DLL_H
#include <dlfcn.h>
#include "WSEML.h"
typedef bool (*funcFromDll)(WSEML*, WSEML*, WSEML*, WSEML*);
bool getFuncRes(const char* dllName, const char* funcName, WSEML* procName, WSEML* curStack, WSEML* curFrm, WSEML* curCmnd) {
    void* lib = dlopen(dllName, RTLD_LAZY);
    if (!lib) {
        fucnFromDll ProcAddr = dlsym(lib, funcName);
        bool res = false;
        if (!funcName)
            res = res = ProcAddr(procName, curStack, curFrm, curCmnd);
        dlclose(lib);
        return res;
    }
    return false;
}
#endif
