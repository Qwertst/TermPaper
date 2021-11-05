#include "WSEML.h"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>

inline WSEML parse(const std::string& text);

namespace{
    WSEML parseHelper(const std::string& text, size_t& curPos);
    WSEML parseBytes (const std::string& text, size_t& curPos){
        std::string str;
        std::unordered_map<char, int> hex = {{'0', 0}, {'1', 1},{ '2', 2}, {'3', 3}, {'4', 4}, {'5', 5}, {'6', 6}, {'7', 7},
                                             {'8', 8}, {'9', 9}, {'a', 10}, {'b', 11}, {'c', 12}, {'d', 13}, {'e', 14}, {'f', 15}};
        unsigned char c = 0;
        int bytes = 0;
        while (text[curPos] != '\"'){
            if (text[curPos] != ' '){
                bytes++;
                if (bytes == 1) c+=16*hex[text[curPos]];
                else c+=hex[text[curPos]];
            }
            if (bytes == 2){
                str+=c;
                c = 0;
                bytes = 0;
            }
            curPos++;
        }
        return WSEML(str);
    }
    WSEML parseList(const std::string& text, size_t& curPos){
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
                if (text[curPos] != '`') {
                    if (text[curPos] == '{') curPos++;
                    size_t balance = 1;
                    for (size_t pos = curPos; (text[pos] != ']' || balance != 0); ++pos) {
                        if (text[pos+1] == '[') balance++;
                        if (text[pos+1] == ']') balance--;
                        if (text[pos] == ':'){ isList = true; break; }
                    }
                }
                if (isList)
                    keyRole = parseList(text, curPos);
                else
                    keyRole = parseHelper(text, curPos);
                std::swap(keyRole, key);
                curPos++;
                if (text[curPos] == ']') curPos++;
                WSEML keyType = parseHelper(text, curPos);
                key.getObj()->setType(keyType);
            }
            curPos++;
            WSEML data = parseHelper(text, curPos);
            if (text[curPos] == '['){
                curPos++;
                bool isList = false;
                if (text[curPos] != '`') {
                    if (text[curPos] == '{') curPos++;
                    size_t balance = 1;
                    for (size_t pos = curPos; (text[pos] != ']' || balance != 0); ++pos) {
                        if (text[pos+1] == '[') balance++;
                        if (text[pos+1] == ']') balance--;
                        if (text[pos] == ':') { isList = true; break; }
                    }
                }
                if (isList)
                    dataRole = parseList(text, curPos);
                else
                    dataRole = parseHelper(text, curPos);
                std::swap(dataRole, data);
                curPos++;
                if (text[curPos] == ']') curPos++;
                WSEML dataType = parseHelper(text, curPos);
                data.getObj()->setType(dataType);
            }
            thisList->get().emplace_back(thisList, key, data, keyRole, dataRole);
        }
        Object* objList = thisList;
        return WSEML(objList);
    }
    WSEML parseHelper(const std::string& text, size_t& curPos){
        while (curPos < text.length()){
            switch(text[curPos]){
                /// Null Object parsing
                case '$': {
                    curPos++;
                    return WSEML();
                }
                    /// String parsing
                case '`': {
                    size_t balance = 1;
                    size_t pos = curPos;
                    for (; (text[pos] != '\'' || balance != 0); ++pos) {
                        if (text[pos+1] == '`') balance++;
                        if (text[pos+1] == '\'') balance--;
                    }
                    std::string str = text.substr(curPos, pos-curPos+1);
                    curPos = pos+1;
                    return WSEML(str);
                }
                    /// Bytes parsing
                case '\"':{
                    curPos++;
                    return parseBytes(text, curPos);
                }
                    /// List parsing
                case '{':{
                    curPos++;
                    return parseList(text, curPos);
                }
                    /// String from file parsing
                case '<':{
                    curPos+=2;
                    size_t pos = text.find('\'', curPos);
                    std::string fileName = text.substr(curPos, pos-curPos);
                    curPos = pos+1;
                    std::ifstream file;
                    file.open(fileName);
                    std::string textFromFile;
                    std::getline(file, textFromFile);
                    return WSEML("`"+textFromFile+"'");
                }
                    /// Substring parsing
                case '#':{
                    curPos++;
                    if (text[curPos] == '<'){
                        curPos+=2;
                        size_t pos = text.find('\'', curPos);
                        std::string path = "C:\\Users\\Stickman\\CLionProjects\\term_paper\\";
                        std::string fileName = path+text.substr(curPos, pos-curPos);
                        curPos = pos+1;
                        std::ifstream file;
                        file.open(fileName);
                        std::string textFromFile;
                        std::getline(file, textFromFile);
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

    bool valid(const std::string& text){
        std::vector<char> queue;
        std::unordered_map<char, char> map = {{'}','{'}, {']','['}, {'\'','`'}, {'\"','\"'}};
        for (size_t pos = 0; pos < text.length(); ++pos){
            if (text[pos] == '\\') pos+=2; // ???
            if (text[pos] == '{' || text[pos] == '[' || text[pos] == '`' || text[pos] == '\"') queue.push_back(text[pos]);
            if (text[pos] == '}' || text[pos] == ']' || text[pos] == '\'' || text[pos] == '\"'){
                if (queue.empty() || map[text[pos]] != queue.back()) return false;
                else queue.pop_back();
            }
        }
        if (queue.empty())
            return true;
        else
            return false;
    }
    std::string packBytes(const std::string& bytes){
        std::string s = "\"";
        std::unordered_map<int, char> hex = {{0, '0'}, {1, '1'},{2, '2'}, {3, '3'}, {4, '4'}, {5, '5'}, {6, '6'}, {7, '7'},
                                             {8, '8'}, {9, '9'}, {10, 'a'}, {11, 'b'}, {12, 'c'}, {13, 'd'}, {14, 'e'}, {15, 'f'}};
        for (size_t pos = 0; pos < bytes.length(); ++pos){
            unsigned char c = bytes[pos];
            std::string byte;
            byte+=hex[c/16];
            byte+=hex[c%16];
            s+=byte;
            if (pos != bytes.length()-1)
                s+=' ';
        }
        s+='\"';
        return s;
    }
}

inline WSEML parse(const std::string& text){
    if (valid(text)) {
        size_t curPos = 0;
        return parseHelper(text, curPos);
    } else std::cout << "Invalid string\n";
    return WSEML();
}

inline std::string pack(WSEML wseml){
    std::string wsemlString;
    if (wseml.getObj() == nullptr) return "$";
    if (wseml.getObj()->typeInfo() == StringType){
        wsemlString = dynamic_cast<ByteString*>(wseml.getObj())->get();
        bool isBytes = false;
        for (char c: wsemlString){
            if (c < 32){
                isBytes = true;
                break;
            }
        }
        if (isBytes) wsemlString = packBytes(wsemlString);
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
