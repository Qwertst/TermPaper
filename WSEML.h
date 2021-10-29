#ifndef WSEML_H
#define WSEML_H
#include <list>
#include <string>

class Pair;
class Object;

class WSEML{
public:
    explicit WSEML(Object* obj): obj(obj){}
    Object* getObj(){ return obj; }
private:
    Object* obj;
};

class Object{
public:
    void setPair(Pair* p){ pair = p; }
    Pair* getPair(){ return pair; }
protected:
    Object(WSEML type, Pair* pair): type(type), pair(pair){};
private:
    WSEML type;
    Pair* pair;
};

class ByteString: public Object{
public:
    ByteString(std::string str, WSEML type, Pair* p = nullptr): Object(type, p), bytes(std::move(str)){};
private:
    std::string bytes;
};

class List: public Object{
public:
    List(std::list<Pair> l, WSEML type, Pair* p = nullptr): Object(type, p), pairList(std::move(l)){};
private:
    std::list<Pair> pairList;
};

class Pair{
public:
    Pair(WSEML key, WSEML data, WSEML keyRole, WSEML dataRole, List* listPtr):
            key(key), data(data), keyRole(keyRole), dataRole(dataRole), listPtr(listPtr){
        key.getObj()->setPair(this);
        data.getObj()->setPair(this);
        keyRole.getObj()->setPair(this);
        dataRole.getObj()->setPair(this);
    }
private:
    WSEML key;
    WSEML data;
    WSEML keyRole;
    WSEML dataRole;
    List* listPtr;
};

const WSEML NULLOBJ = WSEML(nullptr);
#endif