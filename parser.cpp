#include <vector>
#include <unordered_map>
#include <fstream>
#include "WSEML.h"
#include "parser.h"

namespace{
    WSEML parseHelper(std::string& text, size_t& curPos);
    size_t findEnd(std::string& text, size_t& curPos) {
        size_t balance = 1;
        size_t pos = curPos;
        for (; (text[pos] != '\'' || balance != 0); ++pos) {
            if (text[pos] == '\\') pos+=2;
            if (text[pos+1] == '`') balance++;
            if (text[pos+1] == '\'') balance--;
        }
        return pos;
    }
    WSEML parseBytes (std::string& text, size_t& curPos) {
        std::string str;
        std::unordered_map<char, int> hex = {{'0', 0}, {'1', 1},{ '2', 2}, {'3', 3}, {'4', 4}, {'5', 5}, {'6', 6}, {'7', 7},
                                             {'8', 8}, {'9', 9}, {'a', 10}, {'b', 11}, {'c', 12}, {'d', 13}, {'e', 14}, {'f', 15}};
        unsigned char c = 0;
        int bytes = 0;
        while (text[curPos] != '\"') {
            if (text[curPos] != ' ') {
                bytes++;
                if (bytes == 1) c+=16*hex[text[curPos]];
                else c+=hex[text[curPos]];
            }
            if (bytes == 2) {
                str+=c;
                c = 0;
                bytes = 0;
            }
            curPos++;
        }
        return WSEML(str);
    }
    WSEML parseList(std::string& text, size_t& curPos) {
        std::list<Pair> l;
        WSEML ListObj = WSEML(l);
        std::list<Pair>& curList = dynamic_cast<List*>(ListObj.getObj())->get();
        while (text[curPos] != '}' && text[curPos] != ']') {
            if (text[curPos] == ',') {
                curPos++;
                if (text[curPos] == ' ') curPos++;
            }
            WSEML key = parseHelper(text, curPos);
            WSEML keyRole = WSEML();
            WSEML dataRole = WSEML();
            if (text[curPos] == '[') {
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
            if (text[curPos] == '[') {
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
            curList.emplace_back(&ListObj, key, data, keyRole, dataRole);
        }
        curPos++;
        return ListObj;
    }
    WSEML parseHelper(std::string& text, size_t& curPos) {
        while (curPos < text.length()) {
            switch(text[curPos]) {
                /// Null Object parsing
                case '$': {
                    curPos++;
                    return WSEML();
                }
                /// String parsing
                case '`': {
                    size_t pos = findEnd(text, curPos);
                    std::string str = text.substr(curPos+1, pos-curPos-1);
                    curPos = pos+1;
                    return WSEML(str);
                }
                /// Bytes parsing
                case '\"': {
                    curPos++;
                    return parseBytes(text, curPos);
                }
                /// List parsing
                case '{': {
                    curPos++;
                    return parseList(text, curPos);
                }
                /// String from file parsing
                case '<': {
                    curPos+=2;
                    size_t pos = findEnd(text, curPos);
                    std::string fileName = text.substr(curPos, pos-curPos);
                    curPos = pos+1;
                    std::ifstream file;
                    file.open(fileName);
                    std::string textFromFile;
                    std::getline(file, textFromFile);
                    file.close();
                    return WSEML(textFromFile);
                }
                /// Substring parsing
                case '#': {
                    curPos++;
                    if (text[curPos] == '<') {
                        curPos+=2;
                        size_t pos = findEnd(text, curPos);
                        std::string fileName = text.substr(curPos, pos-curPos);
                        curPos = pos+1;
                        std::ifstream file;
                        file.open(fileName);
                        std::string textFromFile;
                        std::getline(file, textFromFile);
                        file.close();
                        return parse(textFromFile);
                    }
                    else {
                        curPos++;
                        size_t pos = findEnd(text, curPos);
                        std::string newText = text.substr(curPos, pos-curPos);
                        curPos = pos+1;
                        return parse(newText);
                    }
                }
                default: {
                    if (text[curPos] == '-' || std::isdigit(text[curPos]) || std::isalpha(text[curPos])) {
                        size_t lastPos = curPos;
                        while (text[curPos] == '-' || std::isalpha(text[curPos]) || std::isdigit(text[curPos]) || text[curPos] == '.' ||
                               text[curPos] == '(' || text[curPos] == ')' || text[curPos] == '/' || text[curPos] == '+')
                            curPos++;
                        std::string str = text.substr(lastPos, curPos-lastPos);
                        return WSEML(str);
                    }
                    return WSEML();
                }
            }
        }
        return WSEML();
    }
//    void validate(std::string& text) {
//        std::vector<char> queue;
//        std::unordered_map<char, char> map = {{'}','{'}, {']','['}, {'\'','`'}, {'\"','\"'}};
//        for (size_t pos = 0; pos < text.length(); ++pos) {
//            if (text[pos] == '\\') pos+=2; // ???
//            if (text[pos] == '{' || text[pos] == '[' || text[pos] == '`' || text[pos] == '\"') queue.push_back(text[pos]);
//            if (text[pos] == '}' || text[pos] == ']' || text[pos] == '\'' || text[pos] == '\"') {
//                if (queue.empty() || map[text[pos]] != queue.back()) return;
//                else queue.pop_back();
//            }
//        }
//    }
    std::string packBytes(std::string& bytes) {
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

WSEML parse(std::string& text) {
//    validate(text);
    size_t curPos = 0;
    return parseHelper(text, curPos);
}

std::string pack(WSEML wseml) {
    std::string wsemlString;
    if (wseml.getObj() == nullptr) return "$";
    if (wseml.getType() == StringType) {
        wsemlString = dynamic_cast<ByteString*>(wseml.getObj())->get();
        bool isBytes = false;
        for (char c: wsemlString) {
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
        for (auto it = list.begin(); it != list.end(); ++it) {
            i++;
            std::string keyRoleStr = pack(it->getKeyRole());
            std::string keyStr = pack(it->getKey());
            std::string keyTypeStr = pack(it->getKey().getObj()->getType());
            if (keyRoleStr != "$" || keyTypeStr !="$")
                keyStr = keyRoleStr + "[" + keyStr + "]" + keyTypeStr;
            std::string dataRoleStr = pack(it->getDataRole());
            std::string dataStr = pack(it->getData());
            std::string dataTypeStr = pack(it->getData().getObj()->getType());
            if (dataRoleStr != "$" || dataTypeStr !="$")
                dataStr = dataRoleStr + "[" + dataStr + "]" + dataTypeStr;
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
