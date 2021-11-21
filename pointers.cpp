#include "pointers.h"
#include <sstream>

WSEML calc(WSEML& expPtr) {
    std::list<Pair>& expList = dynamic_cast<List*>(expPtr.getObj())->get();
    auto listIt = expList.begin();
    std::list<Pair>& compList = dynamic_cast<List*>(listIt->getData().getObj())->get();
    unsigned long long int addr = std::stoi(dynamic_cast<ByteString*>(compList.begin()->getData().getObj())->get(), 0, 16);
    WSEML* curObj = (WSEML*)addr;
    listIt++;
    bool strFlag = false;
    int byteOffset = 0;

    while (listIt != expList.end()) {
        std::list<Pair>& curPS = dynamic_cast<List*>(listIt->getData().getObj())->get();
        auto psIt = curPS.begin();
        std::string step = dynamic_cast<ByteString*>(psIt->getData().getObj())->get();
        if (step == "i") {
            psIt++;
            int index = std::stoi(dynamic_cast<ByteString*>(psIt->getData().getObj())->get());
            if (curObj->getType() == StringType) {
                strFlag = true;
                byteOffset = index;
            }
            else{
                std::list<Pair>& curList = dynamic_cast<List*>(curObj->getObj())->get();
                auto itt = curList.begin();
                std::advance(itt, index);
                curObj = &(itt->getData());
            }
        }
        if (step == "k") {
            psIt++;
            WSEML key = psIt->getData();
            std::list<Pair>& curList = dynamic_cast<List*>(curObj->getObj())->get();
            auto itt = curList.begin();
            while (!equal(itt->getKey(), key)) itt++;
            curObj = &(itt->getData());
        }
        if (step == "u") {
            strFlag = false;
            byteOffset = 0;
            curObj = curObj->getList();
        }
        if (step == "b") {
            psIt++;
            int offset =  std::stoi(dynamic_cast<ByteString*>(psIt->getData().getObj())->get());
            if (curObj->getType() == StringType)
                byteOffset += offset;
            else
                curObj = &((curObj->getObj()->getPair()+offset)->getData());
        }
        listIt++;
    }

    std::ostringstream s;
    s << (void const *)curObj;
    std::string address = s.str();

    std::list<Pair> newCompList;
    WSEML compPtr = WSEML();
    WSEML key = WSEML("addr");
    WSEML data = WSEML(address);
    newCompList.emplace_back(&compPtr, key, data);
    if (strFlag){
        WSEML sKey = WSEML("offset");
        WSEML sData = WSEML(std::to_string(byteOffset));
        newCompList.emplace_back(&compPtr, sKey, sData);
    }
    compPtr = std::move(WSEML(newCompList));
    return compPtr;
}

WSEML expand(WSEML& compPtr) {
    std::list<Pair>& compList = dynamic_cast<List*>(compPtr.getObj())->get();
    std::list<Pair> expList;
    WSEML expPtr = WSEML();
    auto listIt = compList.begin();
    unsigned long long int addr = std::stoi(dynamic_cast<ByteString*>(listIt->getData().getObj())->get(), 0, 16);
    WSEML* curObj = (WSEML*)addr;
    WSEML* upperListPtr = (curObj->getObj()->getPair() != nullptr) ? curObj->getList() : nullptr;

    int countPS = 0;
    listIt++;
    if (listIt != expList.end()) {
        WSEML curPsKey = WSEML(std::to_string(countPS++));
        WSEML type = WSEML("t");
        WSEML byInd = WSEML("i");
        WSEML ps = WSEML();
        std::list<Pair> psList;
        psList.emplace_back(&ps, type, byInd);
        psList.emplace_back(&ps, byInd, listIt->getData());
        ps = std::move(WSEML(psList));
        expList.emplace(expList.begin(), &expPtr, curPsKey, ps);
    }

    while (upperListPtr != nullptr) {
        std::list<Pair>& upperList = dynamic_cast<List*>(upperListPtr->getObj())->get();
        auto it = upperList.begin();
        while (!equal(*curObj, it->getData())) {
            it++;
        }
        WSEML curPsKey = WSEML(std::to_string(countPS++));
        WSEML type = WSEML("t");
        WSEML byKey = WSEML("k");
        WSEML ps = WSEML();
        std::list<Pair> psList;
        psList.emplace_back(&ps, type, byKey);
        psList.emplace_back(&ps, byKey, it->getKey());
        ps = std::move(WSEML(psList));
        expList.emplace(expList.begin(), &expPtr, curPsKey, ps);
        curObj = upperListPtr;
        upperListPtr = (curObj->getObj()->getPair() != nullptr) ? curObj->getList() : nullptr;
    }

    std::ostringstream s;
    s << (void const *)curObj;
    WSEML curPsKey = WSEML(std::to_string(countPS++));
    WSEML type = WSEML("addr");
    WSEML address = WSEML(s.str());
    WSEML ps = WSEML();
    std::list<Pair> psList;
    psList.emplace_back(&ps, type, address);
    ps = std::move(WSEML(psList));
    expList.emplace(expList.begin(), &expPtr, curPsKey, ps);
    expPtr = std::move(WSEML(expList));
    return expPtr;
}

WSEML reduce(WSEML& expPtr) {
    std::list<Pair>& expList = dynamic_cast<List*>(expPtr.getObj())->get();
    auto listIt = expList.begin();
    std::list<Pair>& compList = dynamic_cast<List*>(listIt->getData().getObj())->get();
    unsigned long long int addr = std::stoi(dynamic_cast<ByteString*>(compList.begin()->getData().getObj())->get(), 0, 16);
    WSEML* curObj = (WSEML*)addr;
    WSEML reduced = WSEML();
    std::list<Pair> reducedList;
    reducedList.emplace_back(&reduced, listIt->getKey(), listIt->getData());
    std::list<std::string> psStack = {"0"};
    std::list<WSEML*> objStack = {curObj};
    listIt++;

    while (listIt != expList.end()) {
        std::list<Pair>& curPS = dynamic_cast<List*>(listIt->getData().getObj())->get();
        auto psIt = curPS.begin();
        std::string step = dynamic_cast<ByteString*>(psIt->getData().getObj())->get();
        curObj = objStack.back();
        if (step == "i") {
            psIt++;
            int index = std::stoi(dynamic_cast<ByteString*>(psIt->getData().getObj())->get());

            if (psStack.back() == "i_str"){
                curPS.back().getData() = std::move(WSEML(std::to_string(index)));
                curPS.back().getData().getObj()->setPair(&curPS.back());
                reducedList.back().getData() = std::move(curPS);
                reducedList.back().getData().getObj()->setPair(&reducedList.back());
            }
            else if (psStack.back() == "u" && curObj->getType() == ListType){
                std::list<Pair>& upperList = dynamic_cast<List*>(curObj->getObj())->get();
                auto itOnPrev = upperList.begin();
                objStack.pop_back();
                WSEML* prevObj = objStack.back();
                int prevInd = 0;
                while (!equal(itOnPrev->getData(), *prevObj)){
                    itOnPrev++;
                    prevInd++;
                }
                int offset = index - prevInd;
                auto itOnCur = upperList.begin();
                std::advance(itOnCur, index);
                WSEML newPs = WSEML();
                std::list<Pair> newPsList;
                WSEML type = WSEML("t");
                WSEML toB = WSEML("b");
                WSEML off = WSEML(std::to_string(offset));
                newPsList.emplace_back(&newPs, type, toB);
                newPsList.emplace_back(&newPs, toB, off);
                newPs = std::move(WSEML(newPsList));
                newPs.getObj()->setPair(&reducedList.back());
                reducedList.back().getData() = newPs;
                objStack.push_back(&itOnCur->getData());
            }
            else {
                std::list<Pair>& upperList = dynamic_cast<List*>(curObj->getObj())->get();
                auto itOnPrev = upperList.begin();
                std::advance(itOnPrev, index);
                objStack.push_back(&itOnPrev->getData());
                psStack.push_back(step);
                reducedList.push_back(*listIt);
            }

        } // TODO CHECK
        if (step == "k") {
            std::list<Pair>& upperList = dynamic_cast<List*>(curObj->getObj())->get();
            auto itOnNewKey = upperList.begin();
            WSEML key = curPS.back().getData();
            while (!equal(itOnNewKey->getKey(), key)) itOnNewKey++;

            if (psStack.back() == "u"){
                objStack.pop_back();
                WSEML* prevKey = objStack.back();
                auto itOnPrevKey = upperList.begin();
                while (!equal(itOnPrevKey->getData(), *prevKey)) itOnPrevKey++;
                int offset = std::distance(itOnPrevKey, itOnNewKey);
                if (offset == 0) {
                    psStack.pop_back();
                    reducedList.pop_back();
                }
                else {
                    WSEML newPs = WSEML();
                    std::list<Pair> newPsList;
                    WSEML type = WSEML("t");
                    WSEML toB = WSEML("b");
                    WSEML off = WSEML(std::to_string(offset));
                    newPsList.emplace_back(&newPs, type, toB);
                    newPsList.emplace_back(&newPs, toB, off);
                    newPs = std::move(WSEML(newPsList));
                    newPs.getObj()->setPair(&(reducedList.back()));
                    reducedList.back().getData() = std::move(newPs);
                    objStack.push_back(&itOnNewKey->getData());
                }

            }
            else {
                psStack.push_back(step);
                reducedList.push_back(*listIt);
                objStack.push_back(&itOnNewKey->getData());
            }
        } // TODO CHECK
        if (step == "u") {
            if (psStack.back() == "i" || psStack.back() == "k") {
                psStack.pop_back();
                objStack.pop_back();
                reducedList.pop_back();
            }
            else{
                if (psStack.back() == "b" || psStack.back() == "i_str"){
                    psStack.pop_back();
                    reducedList.pop_back();
                    objStack.pop_back();
                }
                psStack.push_back(step);
                reducedList.emplace_back(&reduced, listIt->getKey(), listIt->getData());
                objStack.push_back(curObj->getList());
            }
        } // TODO CHECK
        if (step == "b") {
            psIt++;
            int offset =  std::stoi(dynamic_cast<ByteString*>(psIt->getData().getObj())->get());
            if (psStack.back() == "b"){
                std::list<Pair>& prevPs = dynamic_cast<List*>(reducedList.back().getData().getObj())->get();
                int prevOffset = std::stoi(dynamic_cast<ByteString*>(prevPs.back().getData().getObj())->get());
                int newOffset = prevOffset+offset;
                if (newOffset != 0){
                    WSEML newOff = WSEML(std::to_string(newOffset));
                    newOff.getObj()->setPair(&(prevPs.back()));
                    prevPs.back().getData() = std::move(newOff);
                    objStack.pop_back();
                    objStack.push_back(&((curObj->getObj()->getPair() + offset)->getData()));
                }
                else{
                    reducedList.pop_back();
                    objStack.pop_back();
                    psStack.pop_back();
                }
            }
            if (psStack.back() == "i_str"){
                std::list<Pair>& prevPs = dynamic_cast<List*>(reducedList.back().getData().getObj())->get();
                int prevOffset = std::stoi(dynamic_cast<ByteString*>(prevPs.back().getData().getObj())->get());
                int newOffset = prevOffset+offset;
                WSEML newOff = WSEML(std::to_string(newOffset));
                newOff.getObj()->setPair(&(prevPs.back()));
                prevPs.back().getData() = std::move(newOff);
            }
            if (psStack.back() == "i"){
                std::list<Pair>& prevPs = dynamic_cast<List*>(reducedList.back().getData().getObj())->get();
                int prevIndex = std::stoi(dynamic_cast<ByteString*>(prevPs.back().getData().getObj())->get());
                int newIndex = prevIndex+offset;
                WSEML newOff = WSEML(std::to_string(newIndex));
                newOff.getObj()->setPair(&(prevPs.back()));
                prevPs.back().getData() = std::move(newOff);

                std::list<Pair>& upperList = dynamic_cast<List*>(curObj->getList()->getObj())->get();
                auto itt = upperList.begin();
                std::advance(itt,newIndex);
                objStack.pop_back();
                objStack.push_back(&(itt->getData()));
            }
            if (psStack.back() == "k"){
                std::list<Pair>& prevPs = dynamic_cast<List*>(reducedList.back().getData().getObj())->get();
                WSEML prevKey = prevPs.back().getData();
                std::list<Pair>& upperList = dynamic_cast<List*>(curObj->getList()->getObj())->get();
                auto itt = upperList.begin();
                while (!equal(itt->getKey(), prevKey)) itt++;
                std::advance(itt, offset);
                WSEML newKey = itt->getKey();
                newKey.getObj()->setPair(&(prevPs.back()));
                prevPs.back().getData() = std::move(newKey);

                objStack.pop_back();
                objStack.push_back(&(itt->getData()));
            }
        } // TODO CHECK
        listIt++;
    }
    reduced = std::move(WSEML(reducedList));
    return reduced;
}

void to_i(WSEML& expPtr) {
    std::list<Pair>& expList = dynamic_cast<List*>(expPtr.getObj())->get();
    auto listIt = expList.begin();
    std::list<Pair>& compList = dynamic_cast<List*>(listIt->getData().getObj())->get();
    unsigned long long int addr = std::stoi(dynamic_cast<ByteString*>(compList.begin()->getData().getObj())->get(), 0, 16);
    WSEML* curObj = (WSEML*)addr;
    listIt++;

    while (listIt != expList.end()) {
        std::list<Pair>& curPS = dynamic_cast<List*>(listIt->getData().getObj())->get();
        auto psIt = curPS.begin();
        std::string step = dynamic_cast<ByteString*>(psIt->getData().getObj())->get();
        if (step == "i") {
            psIt++;
            int index = std::stoi(dynamic_cast<ByteString*>(psIt->getData().getObj())->get());
            if (curObj->getType() == ListType) {
                std::list<Pair>& curList = dynamic_cast<List*>(curObj->getObj())->get();
                auto itt = curList.begin();
                std::advance(itt, index);
                curObj = &(itt->getData());
            }
        }
        if (step == "k") {
            psIt++;
            WSEML key = psIt->getData();
            std::list<Pair>& curList = dynamic_cast<List*>(curObj->getObj())->get();
            auto itt = curList.begin();
            int pos = 0;
            while (!equal(itt->getKey(),key)) {
                pos++;
                itt++;
            }
            curObj = &(itt->getData());

            WSEML newPs = WSEML();
            std::list<Pair> newPsList;
            WSEML type = WSEML("t");
            WSEML byInd = WSEML("i");
            WSEML index = WSEML(std::to_string(pos));
            newPsList.emplace_back(&newPs, type, byInd);
            newPsList.emplace_back(&newPs, byInd, index);
            newPs.getObj()->setPair(&(*listIt));
            newPs = std::move(WSEML(newPsList));
            listIt->getData() = std::move(newPs);
        }
        if (step == "u") {
            curObj = curObj->getList();
        }
        if (step == "b") {
            psIt++;
            int offset =  std::stoi(dynamic_cast<ByteString*>(psIt->getData().getObj())->get());
            if (curObj->getType() == ListType)
                curObj = &((curObj->getObj()->getPair()+offset)->getData());
        }
        listIt++;
    }
}

void to_k(WSEML& expPtr) {
    std::list<Pair>& expList = dynamic_cast<List*>(expPtr.getObj())->get();
    auto listIt = expList.begin();
    std::list<Pair>& compList = dynamic_cast<List*>(listIt->getData().getObj())->get();
    unsigned long long int addr = std::stoi(dynamic_cast<ByteString*>(compList.begin()->getData().getObj())->get(), 0, 16);
    WSEML* curObj = (WSEML*)addr;
    listIt++;

    while (listIt != expList.end()) {
        std::list<Pair>& curPS = dynamic_cast<List*>(listIt->getData().getObj())->get();
        auto psIt = curPS.begin();
        std::string step = dynamic_cast<ByteString*>(psIt->getData().getObj())->get();
        if (step == "i" && curObj->getType() == ListType) {
            psIt++;
            int index = std::stoi(dynamic_cast<ByteString*>(psIt->getData().getObj())->get());
            std::list<Pair>& curList = dynamic_cast<List*>(curObj->getObj())->get();
            auto itt = curList.begin();
            std::advance(itt, index);
            curObj = &(itt->getData());

            WSEML newPs = WSEML();
            std::list<Pair> newPsList;
            WSEML type = WSEML("t");
            WSEML byKey = WSEML("k");
            newPsList.emplace_back(&newPs, type, byKey);
            newPsList.emplace_back(&newPs, byKey, itt->getKey());
            newPs.getObj()->setPair(&(*listIt));
            newPs = std::move(WSEML(newPsList));
            listIt->getData() = std::move(newPs);
        }
        if (step == "k") {
            psIt++;
            WSEML key = psIt->getData();
            std::list<Pair>& curList = dynamic_cast<List*>(curObj->getObj())->get();
            auto itt = curList.begin();
            while (!equal(itt->getKey(), key)) itt++;
            curObj = &(itt->getData());
        }
        if (step == "u") {
            curObj = curObj->getList();
        }
        if (step == "b") {
            psIt++;
            int offset =  std::stoi(dynamic_cast<ByteString*>(psIt->getData().getObj())->get());
            if (curObj->getType() == ListType)
                curObj = &((curObj->getObj()->getPair()+offset)->getData());
        }
        listIt++;
    }
}