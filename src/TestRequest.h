#pragma once

#include "TypedMessage.h"
#include <string>
class TestRequest : public TypedMessage {
public:
    TestRequest(const std::string& rawFixString);
    std::string msgType() const;
    std::string getTestReqID() const;
};
