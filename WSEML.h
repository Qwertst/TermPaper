#ifndef WSEML_H
#define WSEML_H
#include <list>
#include <string>

class Pair;
class WSEML;
class Object;
class ByteString;
class List;

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
    Types getType();
    WSEML* getList();
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
    WSEML& getType();
    virtual bool isSame(Object* obj) = 0;
    virtual bool isSameAs(ByteString* obj) = 0;
    virtual bool isSameAs(List* obj) = 0;
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
    bool isSame(Object* obj) override;
    bool isSameAs(ByteString* obj) override;
    bool isSameAs(List* obj) override;
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
    bool isSame(Object* obj) override;
    bool isSameAs(ByteString* obj) override;
    bool isSameAs(List* obj) override;
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
private:
    WSEML key;
    WSEML data;
    WSEML keyRole;
    WSEML dataRole;
    WSEML* listPtr;
};

bool equal(WSEML& first, WSEML& second);
#endif