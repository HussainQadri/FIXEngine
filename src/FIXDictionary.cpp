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
    }
}

string FIXDictionary::getFieldName(const string& tag) const {
    return m_tagValueMap.at(tag);
}
