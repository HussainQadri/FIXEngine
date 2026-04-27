#pragma once

#include "TypedMessage.h"
class TestRequest : public TypedMessage {
    TestRequest(const string& rawFixString);
    string msgType() const;
    string testReqID() const;
};
