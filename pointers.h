#ifndef POINTERS_H
#define POINTERS_H
#include "WSEML.h"

WSEML calc(WSEML& expPtr);
WSEML expand(WSEML& compPtr);
WSEML reduce(WSEML& expPtr);
void to_i(WSEML& expPtr);
void to_k(WSEML& expPtr);
WSEML* getObjFromPtr(const WSEML& compPtr);
std::string getAddrFromPointer(WSEML* ptr);

#endif
