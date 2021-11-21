#include "WSEML.h"

WSEML NULLOBJ = WSEML();

WSEML::WSEML(Object* obj): obj(obj){}

WSEML::WSEML(): obj(nullptr){}

WSEML::WSEML(std::string str, WSEML &type, Pair *p) {
    obj = new ByteString(std::move(str), type, p);
}

WSEML::WSEML(std::list<Pair> l, WSEML &type, Pair *p) {
    obj = new List(std::move(l), type, p);
}

WSEML::WSEML(const WSEML& wseml) {
    if (wseml.obj != nullptr)
        obj = wseml.obj->clone();
    else
        obj = nullptr;
}

WSEML::WSEML(WSEML&& wseml) noexcept {
    obj = wseml.obj;
    wseml.obj = nullptr;
}

WSEML::~WSEML() {
    delete obj;
}

WSEML& WSEML::operator=(const WSEML& wseml) {
    if (this != &wseml){
        delete obj;
        if (wseml.obj != nullptr)
            obj = wseml.obj->clone();
        else
            obj = nullptr;
    }
    return *this;
}

WSEML& WSEML::operator=(WSEML&& wseml) noexcept {
    if (this != &wseml) {
        delete obj;
        obj = wseml.obj;
        wseml.obj = nullptr;
    }
    return *this;
}

Object* WSEML::getObj() {
    return obj;
}

Types WSEML::getType() {
    return this->getObj()->typeInfo();
}

WSEML* WSEML::getList() {
    return this->getObj()->getPair()->getList();
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

bool ByteString::isSame(Object *obj) {
    return obj->isSameAs(this);
}

bool ByteString::isSameAs(ByteString *obj) {
    return (this->bytes == obj->bytes);
}

bool ByteString::isSameAs(List *obj) {
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

bool List::isSame(Object *obj) {
    return obj->isSameAs(this);
}

bool List::isSameAs(ByteString *obj) {
    return false;
}

bool List::isSameAs(List *obj) {
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


Pair::Pair(WSEML* listPtr, WSEML& key, WSEML& data, WSEML& keyRole, WSEML& dataRole):
        key(key), data(data), keyRole(keyRole), dataRole(dataRole), listPtr(listPtr) {
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

bool equal(WSEML& first, WSEML& second) {
    if (first.getObj() == nullptr && second.getObj() == nullptr) return true;
    if (first.getObj() != nullptr && second.getObj() != nullptr)
        return (first.getObj()->isSame(second.getObj()) && equal(first.getObj()->getType(), second.getObj()->getType()));
    else return false;
}