#include <iostream>
#include "WSEML.h"
#include "parser.h"
#include "pointers.h"
#include <sstream>

int main(){

    std::string str = "{info:{wlist:{1:1}, rot:true}, 1:{info:{wfrm:{1:1}, rot:true, pred:{1:1}, next:{1:1}, origin:first, disp:{t:k, k:kk}}, 1:$[ip:{1:$[t:r]ps, 2:$[t:k, k:prog]ps, 3:$[t:k, k:start]ps}, pred:{1:1}, next:{1:1}]frm}}";
    std::string b = "{1:$[table:{}, data:{}]proc}";
    WSEML wseml = parse(b);
    std::list<Pair>& p = dynamic_cast<List*>(wseml.getObj())->get();
    WSEML& w = p.begin()->getData();
    std::string ws = pack(w);
    std::cout << ws << "\n";
    w.one_step();
    ws = pack(w);
    std::cout << ws;
//    std::ostringstream s;
//    s << (void const *)&wseml;
//    std::string address = s.str();
//    std::string ptrStr = "{0:{addr:" + address + "}, 2:{t:k, i:5}, 1:{t:i, i:1}, 3:{t:k, k:123}}";
//    WSEML expPtr = parse(ptrStr);
//    std::string expStr = pack(expPtr);
//    WSEML comp = calc(expPtr);
//    std::string compStr = pack(comp);
//
//    WSEML newExp = reduce(expPtr);
//    std::list<Pair>& ep = dynamic_cast<List*>(comp.getObj())->get();
//    auto it = ep.begin();
//    WSEML& ps = it->getData();
//    unsigned long long int addr = std::stoi(dynamic_cast<ByteString*>(ps.getObj())->get(), 0, 16);
//    WSEML* w = (WSEML*)addr;
//    std::string www = pack(*w);
//    std::string b = pack(expStr);
//    std::cout << compStr << "\n" << www;
//    std::string cmp = pack(*w);
//    std::cout << expStr << "\n" << compStr << "\n" << cmp;
    return 0;
}