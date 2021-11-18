#include "pointers.h"
#include "parser.h"
#include <sstream>

WSEML calc(WSEML& expPtr) {
    std::list<Pair> l = dynamic_cast<List*>(expPtr.getObj())->get();
    auto listIt = l.begin();
    unsigned long long int addr = std::stoi(dynamic_cast<ByteString*>(listIt->getData().getObj())->get());
    WSEML* curObj = (WSEML*)addr;
    listIt++;
    bool strFlag = false;
    int byteOffset = 0;

    while (listIt != l.end()){
        std::list<Pair> curPS = dynamic_cast<List*>(listIt->getData().getObj())->get();
        auto it = curPS.begin();
        std::string step = dynamic_cast<ByteString*>(it->getData().getObj())->get();
        if (step == "i"){
            it++;
            int index = std::stoi(dynamic_cast<ByteString*>(it->getData().getObj())->get());;
            if (curObj->getObj()->typeInfo() == StringType){
                strFlag = true;
                byteOffset = index;
            }
            else{
                std::list<Pair> curList = dynamic_cast<List*>(curObj->getObj())->get();
                size_t pos = 0;
                auto itt = curList.begin();
                while (itt != curList.end() || pos != index){
                    itt++;
                    pos++;
                }
                curObj = &itt->getData();
            }
        }
        if (step == "k"){ // Only list
            it++;
            WSEML key = it->getData();
            std::string keyStr = pack(key);
            std::list<Pair> curList = dynamic_cast<List*>(curObj->getObj())->get();
            auto itt = curList.begin();
            while (itt != curList.end()) {
                if (pack(itt->getKey()) == keyStr) {
                    curObj = &itt->getData();
                    break;
                }
            }
        }
        if (step == "u"){
            curObj = curObj->getObj()->getPair()->getList();
        }
        if (step == "b"){
            it++;
            int offset =  std::stoi(dynamic_cast<ByteString*>(it->getData().getObj())->get());
            curObj = &((curObj->getObj()->getPair()+offset)->getData());
        }
        listIt++;
    }

    std::ostringstream s;
    s << (void const *)curObj;
    std::string address = s.str();
    std::list<Pair> compList;
    WSEML compPtr = WSEML();
    WSEML key = WSEML("addr");
    WSEML data = WSEML(address);
    compList.emplace_back(&compPtr, key, data);
    if (strFlag){
        WSEML sKey = WSEML("offset");
        WSEML sData = WSEML(std::to_string(byteOffset));
    }
    compPtr = std::move(WSEML(compList));
    return compPtr;
}

WSEML expand(WSEML& compPtr) {
    std::list<Pair> l = dynamic_cast<List*>(compPtr.getObj())->get();
    std::list<Pair> expList;
    WSEML expPtr = WSEML();
    WSEML key = WSEML("1");
    WSEML data = WSEML(l);
    expList.emplace_back(&expPtr, key, data);
    expPtr = std::move(WSEML(expList));
    return expPtr;
}

WSEML reduce(WSEML& expPtr) {
    return WSEML();
}

void to_i(WSEML& expPtr) {
    std::list<Pair> l = dynamic_cast<List*>(expPtr.getObj())->get();
    auto listIt = l.begin();
    unsigned long long int addr = std::stoi(dynamic_cast<ByteString*>(listIt->getData().getObj())->get());
    WSEML* curObj = (WSEML*)addr;
    listIt++;
    while (listIt != l.end()) {
        std::list<Pair> curPS = dynamic_cast<List *>(listIt->getData().getObj())->get();
        auto it = curPS.begin();
        std::string step = dynamic_cast<ByteString *>(it->getData().getObj())->get();
        if (step == "k"){
            it++;
            WSEML key = it->getData();
            std::string keyStr = pack(key);
            std::list<Pair> curList = dynamic_cast<List*>(curObj->getObj())->get();
            auto itt = curList.begin();
            size_t pos = 0;
            while (pack(itt->getKey()) != keyStr) {
                pos++;
            }
            std::string newPSStr = "{t:i, i:";
            newPSStr+=std::to_string(pos);
            newPSStr+="}";
            WSEML newPS = parse(newPSStr);
            listIt->getData() = std::move(newPS);
        }
    }
}

void to_k(WSEML& expPtr) {
    std::list<Pair> l = dynamic_cast<List*>(expPtr.getObj())->get();
    auto listIt = l.begin();
    unsigned long long int addr = std::stoi(dynamic_cast<ByteString*>(listIt->getData().getObj())->get());
    WSEML* curObj = (WSEML*)addr;
    listIt++;
    while (listIt != l.end()) {
        std::list<Pair> curPS = dynamic_cast<List *>(listIt->getData().getObj())->get();
        auto it = curPS.begin();
        std::string step = dynamic_cast<ByteString *>(it->getData().getObj())->get();
        if (step == "i"){
            it++;
            WSEML ind = it->getData();
            size_t index = std::stoi(pack(ind));
            std::list<Pair> curList = dynamic_cast<List*>(curObj->getObj())->get();
            auto itt = curList.begin();
            size_t pos = 0;
            while (pos < index) {
                pos++;
                itt++;
            }
            std::string newPSStr = "{t:k, k:";
            newPSStr+=pack(itt->getKey());
            newPSStr+="}";
            WSEML newPS = parse(newPSStr);
            listIt->getData() = std::move(newPS);
        }
    }
}
