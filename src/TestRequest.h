#pragma once

#include "TypedMessage.h"
class TestRequest : public TypedMessage {
public:
    TestRequest(const string& rawFixString);
    string msgType() const;
    string getTestReqID() const;
};
