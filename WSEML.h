#ifndef WSEML_H
#define WSEML_H
#include <list>
#include <string>

class Pair;
class Object;
class WSEML;

enum Types{ StringType, ListType };

extern WSEML NULLOBJ;

class WSEML{
public:
    WSEML();
    explicit WSEML(Object* obj);
    WSEML(std::string str, WSEML& type = NULLOBJ, Pair* p = nullptr);
    WSEML(std::list<Pair> l, WSEML& type = NULLOBJ, Pair* p = nullptr);
    WSEML(const WSEML& wseml);
    WSEML(WSEML&& wseml) noexcept;
    ~WSEML();
    WSEML& operator=(const WSEML& wseml);
    WSEML& operator=(WSEML&& wseml) noexcept;
    Object* getObj();
private:
    Object* obj;
};

class Object{
public:
    Object(WSEML& type, Pair* pair);
    virtual Object* clone() const = 0;
    virtual ~Object();
    void setPair(Pair* p);
    void setType(WSEML& newType);
    virtual Types typeInfo() const = 0;
    Pair* getPair();
    WSEML getType();
private:
    WSEML type;
    Pair* pair;
};

class ByteString: public Object{
public:
    ByteString(std::string str, WSEML& type = NULLOBJ, Pair* p = nullptr);
    ~ByteString() override;
    ByteString* clone() const override;
    Types typeInfo() const override;
    std::string& get();
private:
    std::string bytes;
};

class List: public Object{
public:
    List(std::list<Pair> l, WSEML& type= NULLOBJ, Pair* p = nullptr);
    ~List() override;
    List* clone() const override;
    Types typeInfo() const override;
    std::list<Pair>& get();
private:
    std::list<Pair> pairList;
};

class Pair{
public:
    Pair(WSEML* listPtr, WSEML& key, WSEML& data, WSEML& keyRole = NULLOBJ, WSEML& dataRole = NULLOBJ);
    WSEML& getKey();
    WSEML& getData();
    WSEML& getKeyRole();
    WSEML& getDataRole();
    WSEML* getList();
    void setList(WSEML* list);
private:
    WSEML key;
    WSEML data;
    WSEML keyRole;
    WSEML dataRole;
    WSEML* listPtr;
};
#endif