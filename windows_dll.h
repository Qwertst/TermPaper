#ifndef WINDOWS_DLL_H
#define WINDOWS_DLL_H
#include <windows.h>
#include "WSEML.h"
typedef bool (*funcFromDll)(WSEML*, WSEML*, WSEML*, WSEML*);
bool getFuncRes(const char* dllName, const char* funcName, WSEML* procName, WSEML* curStack, WSEML* curFrm, WSEML* curCmnd) {
    HINSTANCE lib = LoadLibrary(dllName);
    if (!lib){
        funcFromDll ProcAddr = (funcFromDll)GetProcAddress(lib, funcName);
        bool res = false;
        if (!ProcAddr)
            res = ProcAddr(procName, curStack, curFrm, curCmnd);
        FreeLibrary(lib);
        return res;
    }
    return false;
}
#endif
