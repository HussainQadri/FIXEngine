#pragma once
#include <pugixml.hpp>
#include <set>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
using std::string;
class FIXMessage {
public:
    std::set<std::string> tags;
    std::set<std::string> values;
    std::unordered_map<string, string> m_tagValuePairs;
    FIXMessage(const string& rawFixString);

    string getTagAtIndex(size_t i) const;

    string getValueAtIndex(size_t i) const;

    size_t getFieldCount() const;

    const std::vector<std::pair<string, string>>& getAllFields() const;

    bool validate() const;

    int calculateTotalBytes() const;

    int calculateMessageBodyBytes() const;

    string getValue(const string& tag) const;

    std::vector<string> getValues(const string& tag) const;

private:
    std::vector<std::pair<string, string>> FixMessage;
    void Parse(const string& rawFixString);
    std::pair<string, string> extractChecksumPair() const;
    std::pair<string, string> extractBodyLengthPair() const;
    pugi::xml_document doc;
};
