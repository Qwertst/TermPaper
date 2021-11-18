#include <iostream>
#include "WSEML.h"
#include "parser.h"
#include "pointers.h"

int main(){
    std::string s;
    do{
        std::getline(std::cin, s);
        WSEML wseml = parse(s);
        std::string wsemlStr = pack(wseml);
        std::cout << wsemlStr << "\n";
        std::cout << "--------------------------------------\n";
//        WSEML expPtr = expand(wseml);
//        std::string exp = pack(expPtr);
//        std::cout << exp << "\n";
//        std::cout << "--------------------------------------\n";
    } while (s != "STOP");
    return 0;
}