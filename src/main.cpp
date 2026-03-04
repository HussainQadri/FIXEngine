#include <iostream>
#include <string>
#include <vector>
using std::cout;
using std::string;

class FIXMessage {
  private:
    std::vector<std::pair<string, string>> FixMessage;

  public:
    FIXMessage(std::vector<std::pair<string, string>> message) {
        FixMessage = message;
    }

    string getTagAtIndex(int i) { return FixMessage.at(i).first; }

    string getValueAtIndex(int i) { return FixMessage.at(i).second; }
};

std::vector<std::pair<string, string>> Parse(string rawFixString) {
    std::pair<string, string> pairValue;
    std::vector<std::pair<string, string>> message;
    string tag;
    string value;
    bool readingTag = true;
    for (int i = 0; i < rawFixString.size(); i++) {
        char currentChar = rawFixString[i];
        if (currentChar == '|') {
            pairValue.first = tag;
            pairValue.second = value;
            message.push_back(pairValue);
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
    return message;
}

std::pair<string, string>
extractChecksumPair(std::vector<std::pair<string, string>> parsedMessage) {

    std::pair<string, string> currentPair = parsedMessage.at(0);
    int i = 0;
    while (currentPair.first != "10") {
        i = i + 1;
        currentPair = parsedMessage.at(i);
    }
    return currentPair;
}

int totalBytes(std::vector<std::pair<string, string>> parsedMessage) {
    std::pair<string, string> currentPair = parsedMessage.at(0);
    int i = 0;
    int byteValue = 0;

    while (currentPair.first != "10") {
        for (char c : currentPair.first) {
            byteValue += static_cast<int>(c);
        }

        for (char c : currentPair.second) {
            byteValue += static_cast<int>(c);
        }

        i = i + 1;
        currentPair = parsedMessage.at(i);
    }
    return byteValue;
}
bool validate(std::pair<string, string> checkSumPair, int byteTotal) {
    int checkSum = stoi(checkSumPair.second);
    return checkSum == byteTotal;
}

int main() {
    string testMessage = "8=FIX.4.2|9=49|35=0|49=SENDER|56=TARGET|34=1|52="
                         "20231024-15:30:00.000|10=3220|";
    std::vector<std::pair<string, string>> parseResult = Parse(testMessage);

    for (std::pair<string, string> pair : parseResult) {
        std::cout << "Tag = " << pair.first << " Value = " << pair.second
                  << "\n";
    }

    auto checkSumPair = extractChecksumPair(parseResult);
    int byteValue = totalBytes(parseResult);

    cout << "Checksum Tag: " << checkSumPair.first
         << " Checksum value: " << checkSumPair.second
         << " Byte total: " << byteValue << std::endl;

    cout << "Valid: " << validate(checkSumPair, byteValue);

    FIXMessage fixMessage(parseResult); // useless - change later

    return 0;
}
