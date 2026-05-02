#include "FIXMessage.h"
#include <exception>
#include <iomanip>
#include <sstream>
#include <string>
using std::string;

FIXMessage::FIXMessage() = default;

FIXMessage::FIXMessage(const string& rawFixString) {
    Parse(rawFixString);
}

void FIXMessage::Parse(const string& rawFixString) {
    std::pair<string, string> pairValue;
    string tag;
    string value;
    bool readingTag = true;
    for (int i = 0; i < rawFixString.size(); i++) {
        char currentChar = rawFixString[i];
        if (currentChar == '\x01') {
            pairValue.first = tag;
            pairValue.second = value;
            m_tagValuePairs[tag] = value;
            tags.insert(tag);
            values.insert(value);
            fields.push_back(pairValue);
            tag = "";
            value = "";
            readingTag = true;
        }

        else if (currentChar == '=') {
            readingTag = false;
            continue;
        }

        else {
            if (readingTag) {
                tag += rawFixString[i];
            } else {
                value += rawFixString[i];
            }
        }
    }
}

string FIXMessage::getTagAtIndex(size_t i) const {
    return fields.at(i).first;
}

string FIXMessage::getValueAtIndex(size_t i) const {
    return fields.at(i).second;
}

string FIXMessage::getValue(const string& tag) const {
    if (m_tagValuePairs.contains(tag)) {
        return m_tagValuePairs.at(tag);
    }
    return "";
}

std::vector<string> FIXMessage::getValues(const string& tag) const {
    std::vector<string> res;
    for (const std::pair<string, string>& currentPair : fields) {
        if (currentPair.first == tag) {
            res.push_back(currentPair.second);
        }
    }

    return res;
}

size_t FIXMessage::getFieldCount() const {
    return fields.size();
}

const std::vector<std::pair<string, string>>& FIXMessage::getAllFields() const {
    return fields;
}

std::pair<string, string> FIXMessage::extractBodyLengthPair() const {
    for (const std::pair<string, string>& currentPair : fields) {
        if (currentPair.first == "9") {
            return currentPair;
        }
    }

    return {"", ""};
}

std::pair<string, string> FIXMessage::extractChecksumPair() const {
    for (const std::pair<string, string>& currentPair : fields) {
        if (currentPair.first == "10") {
            return currentPair;
        }
    }
    // Return empty 'pair' for malformed FIX string
    return {"", ""};
}

int FIXMessage::calculateTotalBytes() const {
    std::pair<string, string> currentPair = fields.at(0);
    int i = 0;
    int byteValue = 0;

    while (currentPair.first != "10") {
        for (char c : currentPair.first) {
            byteValue += static_cast<int>(c);
        }
        byteValue += static_cast<int>('=');
        for (char c : currentPair.second) {
            byteValue += static_cast<int>(c);
        }

        byteValue += 1;

        i = i + 1;
        currentPair = fields.at(i);
    }
    return byteValue;
}

int FIXMessage::calculateMessageBodyBytes() const {
    int byteCount = 0;

    for (int i = 2; i < fields.size(); i++) {
        std::pair<string, string> currentPair = fields.at(i);
        if (currentPair.first == "10") {
            break;
        }

        byteCount += currentPair.first.length();
        byteCount += 1;
        byteCount += currentPair.second.length();
        byteCount += 1;
    }
    return byteCount;
}

bool FIXMessage::validate() const {
    std::pair<string, string> checkSumPair = extractChecksumPair();
    std::pair<string, string> bodyLengthPair = extractBodyLengthPair();
    if (checkSumPair.first.empty() || bodyLengthPair.first.empty()) {
        return false;
    }

    try {
        int expectedCheckSum = stoi(checkSumPair.second) % 256;
        int expectedBodyLength = stoi(bodyLengthPair.second);

        int actualCheckSum = calculateTotalBytes() % 256;
        int actualBodyLength = calculateMessageBodyBytes();

        bool checkSumValid = expectedCheckSum == actualCheckSum;
        bool bodyLengthValid = expectedBodyLength == actualBodyLength;

        if (checkSumValid && bodyLengthValid) {
            return true;
        } else {
            return false;
        }

    } catch (const std::exception&) {

        return false;
    }
}

void FIXMessage::appendField(std::string& cur_message, const std::string& tag,
                             const std::string& value) const {
    cur_message += tag + "=" + value + "\x01";
}

std::string FIXMessage::serialize() const {
    string body;
    for (const auto& [tag, value] : fields) {
        if (tag == "8" || tag == "9" || tag == "10") {
            continue;
        }

        appendField(body, tag, value);
    }

    string result;

    appendField(result, "8", getValue("8"));
    appendField(result, "9", std::to_string(body.size()));
    result += body;

    int byte_sum = 0;

    for (unsigned char c : result) {
        byte_sum += c;
    }

    int checksum = byte_sum % 256;

    std::ostringstream oss;
    oss << std::setw(3) << std::setfill('0') << checksum;
    std::string checksumString = oss.str();
    appendField(result, "10", checksumString);

    return result;
}

void FIXMessage::addField(const string& tag, const string& value) {
    fields.push_back({tag, value});
    m_tagValuePairs[tag] = value;
    tags.insert(tag);
    values.insert(value);
}
