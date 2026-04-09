#include "Heartbeat.h"

Heartbeat::Heartbeat()
    : m_mandatoryTags{"8", "9", "35", "49", "56", "34", "52", "10"} {
}

bool Heartbeat::validate() const {
    // will actually implement, validate will check if header and trailer
    // contains the required fields for header and trailer.
    return true;
}

std::string Heartbeat::msgType() const {
    return "0";
}
