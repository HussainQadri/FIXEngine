#pragma once
#include "TypedMessage.h"
#include <string>
class Heartbeat : public TypedMessage {
public:
    Heartbeat(const std::string& rawFixString);
    std::string msgType() const override;
    std::string getTestReqID() const;
};
