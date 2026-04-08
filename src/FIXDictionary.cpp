#include "FIXDictionary.h"
#include <iostream>
#include <pugixml.hpp>
#include <string>
using std::string;

FIXDictionary::FIXDictionary() {
    loadDictionary();
}
void FIXDictionary::loadDictionary() {
    pugi::xml_document doc;
    pugi::xml_parse_result parsed_xml = doc.load_file(FIX_DICTIONARY_PATH);
    pugi::xml_node fields = doc.child("fix").child("fields");
    for (pugi::xml_node field : fields) {
        m_tagValueMap[field.attribute("number").value()] =
            field.attribute("name").value();

        for (pugi::xml_node value : field.children("value")) {
            std::pair<string, string> tagEnumPair;
            tagEnumPair.first = value.attribute("description").value();
            tagEnumPair.second = value.attribute("enum").value();

            auto& enumDescriptionMap =
                m_valueEnumMap[field.attribute("number").value()];
            enumDescriptionMap[tagEnumPair.second] = tagEnumPair.first;
        }
    }
}

string FIXDictionary::getFieldName(const string& tag) const {
    return m_tagValueMap.at(tag);
}

string FIXDictionary::getEnumDescription(const string& tag,
                                         const string& enumValue) const {
    auto query = m_valueEnumMap.find(tag);
    if (query != m_valueEnumMap.end()) {
        return query->second.at(enumValue);
    }
    return "";
}
