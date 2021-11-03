#include "WSEML.h"
#include <iostream>
#include <fstream>

inline WSEML parse(const std::string& text);
inline WSEML parseHelper(const std::string& text, size_t& curPos);

//inline WSEML parseString(const std::string& text, size_t& curPos) {
//    size_t pos = text.find('\'', curPos);
//    std::string str = text.substr(curPos, pos-curPos+1);
//    std::cout << str << "\n";
//    curPos = pos+1;
//    return WSEML(str);
//}

//inline WSEML parseNumber(const std::string& text, size_t& curPos) {
//    size_t lastPos = curPos;
//    while ((48 <= text[curPos] && text[curPos] <= 57) || text[curPos] == 'e' || text[curPos] == '.'
//           || text[curPos] == 'e' || text[curPos] == '(' || text[curPos] == ')' || text[curPos] == '/')
//        curPos++;
//    std::string str = text.substr(lastPos, curPos - lastPos);
//    return WSEML(str);
//}

//inline WSEML parseId(const std::string& text, size_t& curPos) {
//    size_t lastPos = curPos;
//    while ((65 <= text[curPos] && text[curPos] <= 90) || (97 <= text[curPos] && text[curPos] <= 122))
//        curPos++;
//    std::string str = text.substr(lastPos, curPos-lastPos);
//    return WSEML(str);
//}

inline WSEML parseList(const std::string& text, size_t& curPos){
    std::list<Pair> list;
    List* thisList = new List(list);
    while (text[curPos] != '}' && text[curPos] != ']'){
        if (text[curPos] == ',') {
            curPos++;
            if (text[curPos] == ' ') curPos++;
        }
        WSEML key = parseHelper(text, curPos);
        WSEML keyRole = WSEML();
        WSEML dataRole = WSEML();
        if (text[curPos] == '['){
            curPos++;
            bool isList = false;
            if (text[curPos] != '\'') {
                size_t balance = 1;
                for (size_t pos = curPos; (text[pos] != ']' || balance != 0); ++pos) {
                    if (text[pos+1] == '[') balance++;
                    if (text[pos+1] == ']') balance--;
                    if (text[pos] == ':') isList = true;
                }
            }
            if (isList)
                keyRole = parseList(text, curPos);
            else
                keyRole = parseHelper(text, curPos);
            std::swap(keyRole, key);
            curPos++;
            WSEML keyType = parseHelper(text, curPos);
            key.getObj()->setType(keyType);
        }
        curPos++;
        WSEML data = parseHelper(text, curPos);
        if (text[curPos] == '['){
            curPos++;
            bool isList = false;
            if (text[curPos] != '\'') {
                size_t balance = 1;
                for (size_t pos = curPos; (text[pos] != ']' && balance != 0); ++pos) {
                    if (text[pos] == '[') balance++;
                    if (text[pos] == ']') balance--;
                    if (text[pos] == ':') isList = true;
                }
            }
            if (isList)
                dataRole = parseList(text, curPos);
            else
                dataRole = parseHelper(text, curPos);
            std::swap(dataRole, data);
            curPos++;
            WSEML dataType = parseHelper(text, curPos);
            data.getObj()->setType(dataType);
        }
        thisList->get().emplace_back(thisList, key, data, keyRole, dataRole);
    }
    Object* objList = thisList;
    return WSEML(objList);
}

inline WSEML parseHelper(const std::string& text, size_t& curPos){
    while (curPos < text.length()){
        switch(text[curPos]){
            /// Null Object parsing
            case '$': {
                curPos++;
                return WSEML();
            }
            /// String parsing
            case '`': {
                size_t pos = text.find('\'', curPos);
                std::string str = text.substr(curPos, pos-curPos+1);
                std::cout << str << "\n";
                curPos = pos+1;
                return WSEML(str);
            }
            /// Bytes parsing
//            case '\"':{
//                return WSEML(parseBytes(text, curPos));
//            }
            /// List parsing
            case '{':{
                curPos++;
                return parseList(text, curPos);
            }
            case '<':{
                curPos++;
                size_t pos = text.find('\'', curPos);
                std::string newString = text.substr(curPos, pos-curPos);
                curPos = pos+1;
                return parse(newString);
            }
            /// Substring parsing
            case '#':{
                curPos++;
                if (text[curPos] == '<'){
                    curPos+=2;
                    size_t pos = text.find('\'', curPos);
                    std::string fileName = text.substr(curPos, pos-curPos);
                    curPos = pos+1;
                    std::ifstream file;
                    file.open(fileName);
                    std::string textFromFile;
                    std::getline(file, textFromFile);
                    std::cout << textFromFile;
                    return parse(textFromFile);
                }
                else{
                    curPos++;
                    size_t pos = text.find('\'', curPos);
                    std::string newText = text.substr(curPos, pos-curPos);
                    curPos = pos+1;
                    return parse(newText);
                }
            }
            default: {
                /// Numbers parsing
                if (48 <= text[curPos] && text[curPos] <= 57) {
                    size_t lastPos = curPos;
                    while ((48 <= text[curPos] && text[curPos] <= 57) || text[curPos] == 'e' || text[curPos] == '.'
                           || text[curPos] == 'e' || text[curPos] == '(' || text[curPos] == ')' || text[curPos] == '/')
                        curPos++;
                    std::string str = text.substr(lastPos, curPos - lastPos);
                    return WSEML(str);
                }
                /// Identifiers parsing
                if ((65 <= text[curPos] && text[curPos] <= 90) || (97 <= text[curPos] && text[curPos] <= 122)){
                    size_t lastPos = curPos;
                    while ((65 <= text[curPos] && text[curPos] <= 90) || (97 <= text[curPos] && text[curPos] <= 122))
                        curPos++;
                    std::string str = text.substr(lastPos, curPos-lastPos);
                    return WSEML(str);
                }
            }
        }
    }
    return WSEML();
}

inline WSEML parse(const std::string& text){
    size_t curPos = 0;
    return parseHelper(text, curPos);
}

inline std::string pack(WSEML wseml){ // много конструкторов :(
    std::string wsemlString;
    if (wseml.getObj() == nullptr) return "$";
    if (wseml.getObj()->typeInfo() == 0){
        wsemlString = dynamic_cast<ByteString*>(wseml.getObj())->get();
        return wsemlString;
    }
    else{
        std::list<Pair> list = dynamic_cast<List*>(wseml.getObj())->get();
        size_t i = 0;
        wsemlString+="{";
        for (auto it = list.begin(); it != list.end(); ++it){
            i++;
            std::string keyStr = pack(it->getKeyRole()) + "[" + pack(it->getKey()) + "]" + pack(it->getKey().getObj()->getType());
            std::string dataStr = pack(it->getDataRole()) + "[" + pack(it->getData()) + "]" + pack(it->getData().getObj()->getType());
            wsemlString+=keyStr;
            wsemlString+=":";
            wsemlString+=dataStr;
            if (i != list.size())
                wsemlString+=", ";
        }
        wsemlString+="}";
    }
    return wsemlString;
}
