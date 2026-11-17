#include <catch2/catch_test_macros.hpp>

#include "orgcpp/orgcpp.h"

using namespace orgcpp;

TEST_CASE("keyword parsing should work properly", "[orgcpp][keyword]") {
  SECTION("when parsing a basic keyword", "[basic]") {
    RootElement *root =
        orgcpp::parse("./resources/13-keyword/01-basic-keyword.org");
    Keyword *keyword = root->get<Keyword>(0);
    REQUIRE(keyword);
    std::string head = keyword->getAttribute<Region>("head")->string();
    REQUIRE(head == "KEY");
    std::string body = keyword->getAttribute<Region>("body")->string();
    REQUIRE(body == "VALUE");
  }
  SECTION("when parsing a multivalues keyword", "[multivalues]") {
    RootElement *root =
        orgcpp::parse("./resources/13-keyword/02-multivalues-keyword.org");
    Keyword *keyword = root->get<Keyword>(0);
    REQUIRE(keyword);
    std::string head = keyword->getAttribute<Region>("head")->string();
    REQUIRE(head == "KEY");
    std::string body = keyword->getAttribute<Region>("body")->string();
    REQUIRE(body == "VALUE0 VALUE1 VALUE2");
  }  
}
