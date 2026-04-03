#pragma once
#include <pugixml.hpp>
#include <string>
#include <unordered_map>
using std::string;
using std::unordered_map;
class FIXDictionary {
private:
    unordered_map<string, string> m_tagValueMap;
    unordered_map<string, unordered_map<string, string>> m_valueEnumMap;
    pugi::xml_parse_result parsed_xml;

public:
    FIXDictionary();

    bool isValidTag(const string& tag) const;

    bool isValidMsgType(const string& msgType) const;

    string getFieldName(const string& tag) const;

    void loadDictionary();

    void populateDictionary();

    string getEnumDescription(const string& tag, const string& enumValue) const;
};
