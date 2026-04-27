#pragma once
#include "TypedMessage.h"

class Logout : public TypedMessage {
public:
    Logout(const string& rawFixString);
    string msgType() const;
    string getText() const;
    string getEncodedTextLen() const;
    string getEncodedText() const;
};
