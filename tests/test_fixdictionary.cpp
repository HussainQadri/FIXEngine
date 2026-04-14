#include "FIXDictionary.h"
#include "FIXMessage.h"
#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <string>

std::string soh(std::string s) {
    std::replace(s.begin(), s.end(), '|', '\x01');
    return s;
}

TEST_CASE("FIXDictionary loads and resolves tag names", "[dict][lookup]") {
    FIXDictionary dict;

    CHECK(dict.getFieldName("8") == "BeginString");
    CHECK(dict.getFieldName("35") == "MsgType");
    CHECK(dict.getFieldName("49") == "SenderCompID");
    CHECK(dict.getFieldName("10") == "CheckSum");
    CHECK(dict.getFieldName("999999") == "");
}

TEST_CASE("FIXDictionary resolves enum descriptions", "[dict][enum]") {
    FIXDictionary dict;

    CHECK(dict.getEnumDescription("35", "0") == "HEARTBEAT");
    CHECK(dict.getEnumDescription("35", "D") == "NEW_ORDER_SINGLE");
    CHECK(dict.getEnumDescription("35", "ZZZ") == "");
    CHECK(dict.getEnumDescription("999999", "0") == "");
}

TEST_CASE("FIXDictionary isValidTag works", "[dict][lookup]") {
    FIXDictionary dict;

    CHECK(dict.isValidTag("8"));
    CHECK(dict.isValidTag("35"));
    CHECK_FALSE(dict.isValidTag("999999"));
    CHECK_FALSE(dict.isValidTag(""));
}

TEST_CASE("FIXDictionary validates valid heartbeat",
          "[dict][validate][heartbeat]") {
    FIXDictionary dict;
    std::string raw = soh("8=FIX.4.2|"
                          "9=58|"
                          "35=0|"
                          "49=BuySide|"
                          "56=SellSide|"
                          "34=4|"
                          "52=20190605-12:19:52.060|"
                          "10=165|");

    FIXMessage msg(raw);
    REQUIRE(msg.validate());
    CHECK(dict.validate(msg));
}

TEST_CASE("FIXDictionary validates valid NewOrderSingle",
          "[dict][validate][order]") {
    FIXDictionary dict;
    std::string raw = soh("8=FIX.4.2|"
                          "9=118|"
                          "35=D|"
                          "49=ONIXS|"
                          "56=CME|"
                          "34=2|"
                          "52=20240528-09:20:52.111|"
                          "11=983532-3|"
                          "21=1|"
                          "38=100|"
                          "55=NVDA|"
                          "40=1|"
                          "54=1|"
                          "60=20240528-09:20:52.004|"
                          "10=017|");

    FIXMessage msg(raw);
    REQUIRE(msg.validate());
    CHECK(dict.validate(msg));
}

TEST_CASE("FIXDictionary rejects missing required header field SenderCompID",
          "[dict][validate][header]") {
    FIXDictionary dict;
    // Missing tag 49 (SenderCompID)
    std::string raw = soh("8=FIX.4.2|"
                          "9=45|"
                          "35=0|"
                          "56=SellSide|"
                          "34=4|"
                          "52=20190605-12:19:52.060|"
                          "10=052|");

    FIXMessage msg(raw);
    CHECK_FALSE(dict.validate(msg));
}

TEST_CASE("FIXDictionary rejects missing required header field TargetCompID",
          "[dict][validate][header]") {
    FIXDictionary dict;
    // Missing tag 56 (TargetCompID)
    std::string raw = soh("8=FIX.4.2|"
                          "9=45|"
                          "35=0|"
                          "49=BuySide|"
                          "34=4|"
                          "52=20190605-12:19:52.060|"
                          "10=190|");

    FIXMessage msg(raw);
    CHECK_FALSE(dict.validate(msg));
}

TEST_CASE("FIXDictionary rejects missing required trailer field CheckSum",
          "[dict][validate][trailer]") {
    FIXDictionary dict;
    // No tag 10 — structural validate also fails but dict should catch it
    std::string raw = soh("8=FIX.4.2|"
                          "9=58|"
                          "35=0|"
                          "49=BuySide|"
                          "56=SellSide|"
                          "34=4|"
                          "52=20190605-12:19:52.060|");

    FIXMessage msg(raw);
    CHECK_FALSE(dict.validate(msg));
}
