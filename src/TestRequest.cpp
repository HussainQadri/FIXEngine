#include "TestRequest.h"
#include "TypedMessage.h"

TestRequest::TestRequest(const string& rawFixString)
    : TypedMessage(rawFixString) {
    if (getValue("35") != "1") {
        throw std::invalid_argument("Not a TestRequest");
    }
}

std::string TestRequest::msgType() const {
    return "1";
}

std::string TestRequest::getTestReqID() const {
    return getValue("112");
}
