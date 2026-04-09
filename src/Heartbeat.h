#pragma once
#include "TypedMessage.h"
#include <set>
#include <string>
#include <unordered_map>
class Heartbeat : public TypedMessage {
public:
    Heartbeat(const std::string& rawFixString);
    std::string msgType() const override;
    bool validate() const override;
    std::unordered_map<std::string, std::string> requiredKeyValuePairs;
    bool containsAllMandatoryTags(std::set<std::string> m_mandatoryTags);

private:
    std::set<std::string> m_mandatoryTags;
};
