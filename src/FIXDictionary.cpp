#include "FIXDictionary.h"
#include <iostream>
#include <pugixml.hpp>
#include <string>
#include <unordered_map>
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

        m_nameTagMap[field.attribute("name").value()] =
            field.attribute("number").value();

        for (pugi::xml_node value : field.children("value")) {
            std::pair<string, string> tagEnumPair;
            tagEnumPair.first = value.attribute("description").value();
            tagEnumPair.second = value.attribute("enum").value();

            auto& enumDescriptionMap =
                m_valueEnumMap[field.attribute("number").value()];
            enumDescriptionMap[tagEnumPair.second] = tagEnumPair.first;
        }
    }

    loadHeaderFields(doc);
    loadTrailerFields(doc);
    loadMessages(doc);
}

void FIXDictionary::loadHeaderFields(const pugi::xml_document& doc) {
    pugi::xml_node headerFields = doc.child("fix").child("header");
    for (pugi::xml_node headerField : headerFields) {
        m_headerFields[headerField.attribute("name").value()] =
            headerField.attribute("required").value();
    }
}

void FIXDictionary::loadTrailerFields(const pugi::xml_document& doc) {
    pugi::xml_node trailerFields = doc.child("fix").child("trailer");
    for (pugi::xml_node trailerField : trailerFields) {
        m_trailerFields[trailerField.attribute("name").value()] =
            trailerField.attribute("required").value();
    }
}

void FIXDictionary::loadMessages(const pugi::xml_document& doc) {
    pugi::xml_node messageFields = doc.child("fix").child("messages");

    for (pugi::xml_node messageType : messageFields) {
        for (pugi::xml_node messageField : messageType) {

            std::unordered_map<string, string>& requiredMsgTypeFields =
                m_messageFields[messageType.attribute("msgtype").value()];

            requiredMsgTypeFields[messageField.attribute("name").value()] =
                messageField.attribute("required").value();
        }
    }
}

string FIXDictionary::getFieldName(const string& tag) const {
    if (isValidTag(tag)) {
        return m_tagValueMap.at(tag);
    }
    return "";
}

string FIXDictionary::getEnumDescription(const string& tag,
                                         const string& enumValue) const {
    auto tagIt = m_valueEnumMap.find(tag);
    if (tagIt == m_valueEnumMap.end()) {
        return "";
    }
    auto valueIt = tagIt->second.find(enumValue);
    if (valueIt == tagIt->second.end()) {
        return "";
    }
    return valueIt->second;
}

bool FIXDictionary::isValidTag(const string& tag) const {
    if (m_tagValueMap.find(tag) != m_tagValueMap.end()) {
        return true;
    }

    return false;
}

bool FIXDictionary::validate(const FIXMessage& fixMessage) const {
    // Validate header
    for (const std::pair<string, string>& tagValuePair : m_headerFields) {
        const string& tag = m_nameTagMap.at(tagValuePair.first);
        if (tagValuePair.second == "Y" && fixMessage.getValue(tag) == "") {
            return false;
        }
    }

    // Validate message body
    string msgType = fixMessage.getValue("35");
    unordered_map<string, string> requiredFieldsMap =
        m_messageFields.at(msgType); // this can throw
    for (const std::pair<string, string>& nameRequiredPair :
         requiredFieldsMap) {
        const string& tag = m_nameTagMap.at(nameRequiredPair.first);
        if (nameRequiredPair.second == "Y" && fixMessage.getValue(tag) == "") {
            return false;
        }
    }

    // Validate trailer
    for (const std::pair<string, string>& tagValuePair : m_trailerFields) {
        const string& tag = m_nameTagMap.at(tagValuePair.first);
        if (tagValuePair.second == "Y" && fixMessage.getValue(tag) == "") {
            return false;
        }
    }

    return true;
}
