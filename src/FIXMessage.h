#pragma once
#include <pugixml.hpp>
#include <set>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
class FIXMessage {
public:
    FIXMessage();

    FIXMessage(const std::string& rawFixString);

    std::string getTagAtIndex(size_t i) const;

    std::string getValueAtIndex(size_t i) const;

    size_t getFieldCount() const;

    const std::vector<std::pair<std::string, std::string>>&
    getAllFields() const;

    bool validate() const;

    int calculateTotalBytes() const;

    int calculateMessageBodyBytes() const;

    std::string getValue(const std::string& tag) const;

    std::vector<std::string> getValues(const std::string& tag) const;

    std::string serialize() const;

    void addField(const std::string& tag, const std::string& value);

private:
    void appendField(std::string& cur_message, const std::string& tag,
                     const std::string& value) const;
    std::vector<std::pair<std::string, std::string>> fields;
    void Parse(const std::string& rawFixString);
    std::pair<std::string, std::string> extractChecksumPair() const;
    std::pair<std::string, std::string> extractBodyLengthPair() const;
    pugi::xml_document doc;
    std::set<std::string> tags;
    std::set<std::string> values;
    std::unordered_map<std::string, std::string> m_tagValuePairs;
};
