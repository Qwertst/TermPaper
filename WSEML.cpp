#include "WSEML.h"

WSEML::WSEML(Object *obj): obj(obj){}

WSEML::WSEML(): obj(nullptr){}

WSEML::WSEML(const WSEML &wseml){
    if (wseml.obj != nullptr)
        obj = wseml.obj->clone();
    else
        obj = nullptr;
}

WSEML::WSEML(WSEML &&wseml) noexcept {
    obj = wseml.obj;
    wseml.obj = nullptr;
}

WSEML::~WSEML() {
    delete obj;
}

WSEML& WSEML::operator=(const WSEML &wseml) {
    if (this != &wseml){
        delete obj;
        if (wseml.obj != nullptr)
            obj = wseml.obj->clone();
        else
            obj = nullptr;
    }
    return *this;
}

WSEML &WSEML::operator=(WSEML &&wseml) noexcept {
    if (this != &wseml) {
        delete obj;
        obj = wseml.obj;
        wseml.obj = nullptr;
    }
    return *this;
}

Object *WSEML::getObj() {
    return obj;
}

Object::Object(WSEML &type, Pair *pair): type(type), pair(pair){}

Object::~Object() = default;

void Object::setPair(Pair *p) {
    pair = p;
}

Pair *Object::getPair() {
    return pair;
}

ByteString::ByteString(std::string str, WSEML& type, Pair *p): Object(type, p), bytes(std::move(str)) {}

ByteString::~ByteString() = default;

ByteString* ByteString::clone() const {
    return new ByteString(*this);
}

List::List(std::list<Pair> l, WSEML& type, Pair *p): Object(type, p), pairList(std::move(l)) {}

List::~List() = default;

List* List::clone() const {
    return new List(*this);
}

Pair::Pair(WSEML& key, WSEML& data, WSEML& keyRole, WSEML& dataRole, List *listPtr):
        key(key), data(data), keyRole(keyRole), dataRole(dataRole), listPtr(listPtr){
    key.getObj()->setPair(this);
    data.getObj()->setPair(this);
    keyRole.getObj()->setPair(this);
    dataRole.getObj()->setPair(this);
}