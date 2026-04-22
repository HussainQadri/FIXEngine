#include "TypedMessage.h"

class SequenceReset : public TypedMessage {
    SequenceReset(const string& rawFixString);
    string msgType() const;
};
