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

WSEML::WSEML(const WSEML& wseml){
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

Object::Object(WSEML& type, Pair* pair): type(type), pair(pair){}

Object::~Object() = default;

void Object::setPair(Pair* p) {
    pair = p;
}

Pair* Object::getPair() {
    return pair;
}

WSEML Object::getType() {
    return type;
}

void Object::setType(WSEML& newType) {
    type = newType;
}

ByteString::ByteString(std::string str, WSEML& type, Pair* p): Object(type, p), bytes(std::move(str)) {}

ByteString::~ByteString() = default;

ByteString* ByteString::clone() const {
    return new ByteString(*this);
}

std::string &ByteString::get() {
    return bytes;
}

int ByteString::typeInfo() const {
    return 0;
}

List::List(std::list<Pair> l, WSEML& type, Pair* p): Object(type, p), pairList(std::move(l)) {}

List::~List() = default;

List* List::clone() const {
    return new List(*this);
}

std::list<Pair> &List::get() {
    return pairList;
}

int List::typeInfo() const {
    return 1;
}

Pair::Pair(List* listPtr, WSEML& key, WSEML& data, WSEML& keyRole, WSEML& dataRole):
        key(key), data(data), keyRole(keyRole), dataRole(dataRole), listPtr(listPtr){
    if (key.getObj())
        key.getObj()->setPair(this);
    if (data.getObj())
        data.getObj()->setPair(this);
    if (keyRole.getObj())
        keyRole.getObj()->setPair(this);
    if (dataRole.getObj())
        dataRole.getObj()->setPair(this);
}

WSEML Pair::getKey() {
    return key;
}

WSEML Pair::getData() {
    return data;
}

WSEML Pair::getKeyRole(){
    return keyRole;
}

WSEML Pair::getDataRole() {
    return dataRole;
}
