#include "WSEML.h"
#include "pointers.h"
#include "parser.h"
#include "dllconfig.h"

WSEML NULLOBJ = WSEML();

WSEML::WSEML(Object* obj): obj(obj){}

WSEML::WSEML(): obj(nullptr){}

WSEML::WSEML(std::string str, WSEML &type, Pair *p) {
    obj = new ByteString(str, type, p);
}

WSEML::WSEML(std::list<Pair> l, WSEML &type, Pair *p) {
    List* lst = new List(l, type, p);
    obj = lst;
    for (Pair& pair : lst->get())
        pair.setList(this);
}

WSEML::WSEML(const WSEML& wseml) {
    if (wseml.obj != nullptr) {
        obj = wseml.obj->clone();
        if (obj->typeInfo() == ListType) {
            std::list<Pair>& list = dynamic_cast<List*>(obj)->get();
            for (Pair &pair : list) {
                pair.setList(this);
            }
        }
    }
    else
        obj = nullptr;
}

WSEML::WSEML(WSEML&& wseml) noexcept {
    obj = wseml.obj;
    if (obj != nullptr && obj->typeInfo() == ListType) {
        std::list<Pair>& list = dynamic_cast<List *>(obj)->get();
        for (Pair &pair : list) {
            pair.setList(this);
        }
    }
    wseml.obj = nullptr;
}

WSEML::~WSEML() {
    delete obj;
}

WSEML& WSEML::operator=(const WSEML& wseml) {
    if (this != &wseml){
        delete obj;
        if (wseml.obj != nullptr) {
            obj = wseml.obj->clone();
            if (obj->typeInfo() == ListType) {
                std::list<Pair>& list = dynamic_cast<List*>(obj)->get();
                for (Pair& pair : list) {
                    pair.setList(this);
                }
            }
        }
        else
            obj = nullptr;
    }
    return *this;
}

WSEML& WSEML::operator=(WSEML&& wseml) noexcept {
    if (this != &wseml) {
        delete obj;
        obj = wseml.obj;
        if (obj != nullptr && obj->typeInfo() == ListType) {
            std::list<Pair>& list = dynamic_cast<List*>(obj)->get();
            for (Pair& pair : list) {
                pair.setList(this);
            }
        }
        wseml.obj = nullptr;
    }
    return *this;
}

Object* WSEML::getObj() const {
    return obj;
}

Types WSEML::typeInfo() const {
    return this->getObj()->typeInfo();
}

WSEML &WSEML::getType() {
    return this->getObj()->getType();
}

WSEML* WSEML::getList() {
    return (this->getObj()->getPair() != nullptr) ? this->getObj()->getPair()->getList() : nullptr;
}

Object::Object(WSEML& type, Pair* pair): type(type), pair(pair){}

Object::~Object() = default;

void Object::setPair(Pair* p) {
    pair = p;
}

Pair* Object::getPair() {
    return pair;
}

WSEML& Object::getType() {
    return type;
}

void Object::setType(WSEML& newType) {
    type = newType;
}

ByteString::ByteString(std::string str, WSEML& type, Pair* p): Object(type, p), bytes(std::move(str)){}

ByteString::~ByteString() = default;

ByteString* ByteString::clone() const {
    return new ByteString(*this);
}

std::string& ByteString::get() {
    return bytes;
}

Types ByteString::typeInfo() const {
    return StringType;
}

bool ByteString::equal(Object *obj) {
    return obj->equalTo(this);
}

bool ByteString::equalTo(ByteString *obj) {
    return (this->bytes == obj->bytes) && equal(this->getType(), obj->getType());
}

bool ByteString::equalTo(List *obj) {
    return false;
}

List::List(std::list<Pair> l, WSEML& type, Pair* p): Object(type, p), pairList(std::move(l)){}

List::~List() = default;

List* List::clone() const {
    return new List(*this);
}

std::list<Pair>& List::get() {
    return pairList;
}

Types List::typeInfo() const {
    return ListType;
}

bool List::equal(Object *obj) {
    return obj->equalTo(this);
}

bool List::equalTo(ByteString *obj) {
    return false;
}

bool List::equalTo(List *obj) {
    bool flag = true;
    auto firstIt = this->pairList.begin();
    auto secondIt = obj->pairList.begin();
    for (; firstIt != this->pairList.end() && secondIt != obj->pairList.end() && flag; firstIt++, secondIt++) {
        flag = (equal(firstIt->getKey(), secondIt->getKey()) && equal(firstIt->getData(), secondIt->getData()) &&
                equal(firstIt->getKeyRole(), secondIt->getKeyRole()) && equal(firstIt->getDataRole(), secondIt->getDataRole()));
    }
    if (firstIt != this->pairList.end() || secondIt != obj->pairList.end()) flag = false;
    return flag;
}

WSEML List::genKey() {
    WSEML key = WSEML(std::to_string(this->nextKey));
    this->nextKey+=2;
    return key;
}

unsigned int &List::getCurMaxKey() {
    return this->nextKey;
}


Pair::Pair(WSEML* listPtr, WSEML key, WSEML data, WSEML keyRole, WSEML dataRole):
        key(std::move(key)), data(std::move(data)), keyRole(std::move(keyRole)), dataRole(std::move(dataRole)), listPtr(listPtr) {
    if (this->key.getObj())
        this->key.getObj()->setPair(this);
    if (this->data.getObj())
        this->data.getObj()->setPair(this);
    if (this->keyRole.getObj())
        this->keyRole.getObj()->setPair(this);
    if (this->dataRole.getObj())
        this->dataRole.getObj()->setPair(this);
}

WSEML& Pair::getKey() {
    return key;
}

WSEML& Pair::getData() {
    return data;
}

WSEML& Pair::getKeyRole() {
    return keyRole;
}

WSEML& Pair::getDataRole() {
    return dataRole;
}

WSEML* Pair::getList() {
    return listPtr;
}

void Pair::setList(WSEML *lst) {
    this->listPtr = lst;
}

Pair::Pair(const Pair& p): key(p.key), data(p.data), keyRole(p.keyRole), dataRole(p.dataRole) {
    if (this->key.getObj())
        this->key.getObj()->setPair(this);
    if (this->data.getObj())
        this->data.getObj()->setPair(this);
    if (this->keyRole.getObj())
        this->keyRole.getObj()->setPair(this);
    if (this->dataRole.getObj())
        this->dataRole.getObj()->setPair(this);
}

bool equal(const WSEML& first, const WSEML& second) {
    if (first.getObj() == nullptr && second.getObj() == nullptr) return true;
    if (first.getObj() != nullptr && second.getObj() != nullptr)
        return (first.getObj()->equal(second.getObj()) && equal(first.getObj()->getType(), second.getObj()->getType()));
    else return false;
}

bool WSEML::one_step() {
    std::list<Pair>& thisProcess = dynamic_cast<List*>(this->obj)->get();
    auto procIt = thisProcess.begin();
    while (procIt != thisProcess.end() && !equal(procIt->getKey(), WSEML("stck"))) procIt++;
    if (procIt == thisProcess.end()) {
        WSEML initStack = parse("{info:{wlist:{1:1}, rot:true}, 1:{info:{wfrm:{1:1}, rot:true, pred:{}, next:{}, origin:first, disp:{}},"
                                "1:$[ip:{1:$[t:r]ps, 2:$[t:k, k:prog]ps, 3:$[t:k, k:start]ps}, pred:{}, next:{}]frm}}");
        WSEML stck = WSEML("stck");
        thisProcess.emplace_back(this, stck, initStack);
        return true;
    }
    else {
        bool stepRes = false;
        std::list<Pair>& stck = dynamic_cast<List*>(procIt->getData().getObj())->get();
        auto stckIt = stck.begin();
        while (!equal(stckIt->getKey(), WSEML("info"))) stckIt++;
        std::list<Pair>& curStckInfo = dynamic_cast<List*>(stckIt->getData().getObj())->get();

        auto curStckInfoIt = curStckInfo.begin();
        while (!equal(stckIt->getKey(), WSEML("wlist"))) curStckInfoIt++;
        std::list<Pair>& wlist = dynamic_cast<List*>(curStckInfoIt->getData().getObj())->get();
        WSEML& StackKey = wlist.begin()->getData();

        procIt = thisProcess.begin();
        while (!equal(procIt->getKey(), WSEML("table"))) procIt++;
        std::list<Pair>& table = dynamic_cast<List*>(procIt->getData().getObj())->get();
        auto tableIt = table.begin();

        stckIt = stck.begin();
        while(!equal(StackKey, stckIt->getKey())) stckIt++;
        std::list<Pair>& curStack = dynamic_cast<List*>(stckIt->getData().getObj())->get();

        auto curStackIt = curStack.begin();
        while (!equal(curStackIt->getKey(), WSEML("info"))) curStackIt++;
        std::list<Pair>& curStackInfo = dynamic_cast<List*>(curStackIt->getData().getObj())->get();

        auto curStackInfoIt = curStackInfo.begin();
        while (!equal(curStackInfoIt->getKey(), WSEML("wfrm"))) curStackInfoIt++;
        std::list<Pair>& wfrm = dynamic_cast<List*>(curStackInfoIt->getData().getObj())->get();
        WSEML& FrameKey = wfrm.begin()->getData();

//        bool frmRot = (equal(std::next(curStackInfo.begin())->getData(), WSEML("true")) ? true : false);
//        if (frmRot) wfrm.emplace_back(*wfrm.begin());
//        wfrm.pop_front();

        curStackIt = curStack.begin();
        while(!equal(FrameKey, curStackIt->getKey())) curStackIt++;
        std::list<Pair>& curFrame = dynamic_cast<List*>(curStackIt->getData().getObj())->get();
        auto curFrameIt = curFrame.begin();
        while(!equal(curFrameIt->getKey(), WSEML("ip"))) curFrameIt++;
        WSEML& ip = curFrameIt->getData();
        WSEML* curCommand = getObjFromPtr(calc(ip));
        WSEML& commandType = curCommand->getType();

        while(!equal(tableIt->getKey(), commandType)) tableIt++;
        std::list<Pair>& curCommandTable = dynamic_cast<List*>(tableIt->getData().getObj())->get();
        auto curTableIt = curCommandTable.end();
        curTableIt--;

        if (equal(commandType, WSEML("bc"))) {
            std::list<Pair>& bc = dynamic_cast<List*>(curTableIt->getData().getObj())->get(); // TODO: Как хранится: bc:{dllName:funcName}?
            std::string dllName = dynamic_cast<ByteString*>(bc.begin()->getKey().getObj())->get();
            std::string funcName = dynamic_cast<ByteString*>(bc.begin()->getData().getObj())->get();
            stepRes = getFuncRes(dllName.c_str(), funcName.c_str(), this, &stckIt->getData(), &curStackIt->getData(), &tableIt->getData());
        }
        else {
            std::list<Pair>& cmnd = dynamic_cast<List*>(curTableIt->getData().getObj())->get();
            auto cmndIt = cmnd.begin();
            if (equal(curTableIt->getData().getType(), WSEML("embd"))) {
                std::string dllName = dynamic_cast<ByteString*>(cmndIt->getKey().getObj())->get();
                std::string funcName = dynamic_cast<ByteString*>(cmndIt->getData().getObj())->get();
                stepRes = getFuncRes(dllName.c_str(), funcName.c_str(), this, &stckIt->getData(), &curStackIt->getData(), &tableIt->getData());
            }
            else { // Implemented command, type = impl
                while(!equal(cmndIt->getKey(), WSEML("start"))) cmndIt++;
                WSEML* startIp = &cmndIt->getData();
                std::string startAddr = getAddrFromPointer(startIp);

                std::string prevKey = pack(wlist.begin()->getKey());
                std::string prevData = pack(wlist.begin()->getData());
                wlist.pop_front();

                std::string newStackStr = "{info:{wfrm:{1:1}, rot:true, pred:{" + prevKey + ":" + prevData
                        + "}, next:{}, origin:nd, disp:{}}, 1:$[ip:{1:$[addr:" + startAddr + "]compPtr, 2:$[t:k, k:start]ps}, pred:{}, next:{}]frm}}";

                WSEML newStack = parse(newStackStr);
                List* stckList = dynamic_cast<List*>(procIt->getData().getObj());
                WSEML newStackKey = stckList->genKey(); // TODO: test key generation
                stepRes = true;
            }
        }

        if (wfrm.empty()) {
            bool stckRot = (equal(curStckInfo.back().getData(), WSEML("true")) ? true : false);
            if (stckRot) wlist.emplace_back(*wlist.begin());
            wlist.pop_front();
        }
        return stepRes;
    }
}