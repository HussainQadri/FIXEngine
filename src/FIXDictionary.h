#pragma once
#include <pugixml.hpp>
#include <string>
#include <unordered_map>

using std::string;
using std::unordered_map;
class FIXDictionary {
private:
    unordered_map<string, string> m_tagValueMap;
    pugi::xml_parse_result parsed_xml;

public:
    FIXDictionary();

    bool isValidTag(const string& tag) const;

    bool isValidMsgType(const string& msgType) const;

    string getFieldName(const string& tag) const;

    void loadDictionary();

    void populateDictionary();
};
