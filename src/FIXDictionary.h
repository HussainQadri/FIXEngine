#pragma once
#include "FIXMessage.h"
#include <pugixml.hpp>
#include <string>
#include <unordered_map>
class FIXDictionary {
private:
    // Tag is a number, value is what that number represents
    std::unordered_map<std::string, std::string> m_tagValueMap;
    std::unordered_map<std::string,
                       std::unordered_map<std::string, std::string>>
        m_valueEnumMap;
    std::unordered_map<std::string, std::string> m_headerFields;
    std::unordered_map<std::string, std::string> m_trailerFields;
    std::unordered_map<std::string,
                       std::unordered_map<std::string, std::string>>
        m_messageFields;
    std::unordered_map<std::string, std::string> m_nameTagMap;
    pugi::xml_parse_result parsed_xml;

public:
    FIXDictionary();

    bool isValidTag(const std::string& tag) const;

    std::string getFieldName(const std::string& tag) const;

    void loadDictionary();

    void loadHeaderFields(const pugi::xml_document& doc);

    void loadTrailerFields(const pugi::xml_document& doc);

    void loadMessages(const pugi::xml_document& doc);

    std::string getEnumDescription(const std::string& tag,
                                   const std::string& enumValue) const;

    bool validate(const FIXMessage& fixMessage) const;

    std::string nameToTag(const std::string& name) const;
};
