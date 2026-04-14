#pragma once
#include "FIXMessage.h"
#include <pugixml.hpp>
#include <string>
#include <unordered_map>
using std::string;
using std::unordered_map;
class FIXDictionary {
private:
    // Tag is a number, value is what that number represents
    unordered_map<string, string> m_tagValueMap;
    unordered_map<string, unordered_map<string, string>> m_valueEnumMap;
    unordered_map<string, string> m_headerFields;
    unordered_map<string, string> m_trailerFields;
    unordered_map<string, unordered_map<string, string>> m_messageFields;
    unordered_map<string, string> m_nameTagMap;
    pugi::xml_parse_result parsed_xml;

public:
    FIXDictionary();

    bool isValidTag(const string& tag) const;

    string getFieldName(const string& tag) const;

    void loadDictionary();

    void loadHeaderFields(const pugi::xml_document& doc);

    void loadTrailerFields(const pugi::xml_document& doc);

    void loadMessages(const pugi::xml_document& doc);

    string getEnumDescription(const string& tag, const string& enumValue) const;

    bool validate(const FIXMessage& fixMessage) const;
};
