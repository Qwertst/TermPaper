#include "WSEML.h"
#include "parser.cpp"
#include <iostream>

int main(){
    std::string s;
    do{
        std::getline(std::cin, s);
        WSEML wseml = parse(s);
        std::string wsemlStr = pack(wseml);
        std::cout << wsemlStr << "\n";
        std::cout << "--------------------------------------\n";
    } while (s != "STOP");
    return 0;
}