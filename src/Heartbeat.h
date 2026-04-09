#pragma once
#include "TypedMessage.h"
#include <set>
#include <string>
#include <unordered_map>
class Heartbeat : TypedMessage {
public:
    Heartbeat();
    std::string msgType() const override;
    bool validate() const override;
    std::unordered_map<std::string, std::string> requiredKeyValuePairs;

private:
    std::set<std::string> m_mandatoryTags;
};
