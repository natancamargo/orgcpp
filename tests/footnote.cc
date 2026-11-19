#include <catch2/catch_test_macros.hpp>

#include "orgcpp/orgcpp.h"

using namespace orgcpp;

TEST_CASE("footnote parsing should work properly", "[orgcpp][footnote]") {
  SECTION("when parsing a basic footnote", "[basic]") {
    RootElement *root =
        orgcpp::parse("./resources/12-footnote/01-basic-footnote.org");
    Footnote *footnote = root->get<Footnote>(0);
    REQUIRE(footnote);
    std::string head = footnote->getAttribute<Region>("head")->string();
    REQUIRE(head == ":text");
    std::string content = footnote->getAttribute<Region>("content")->string();
    REQUIRE(content == "[fn:text]");
  }
  SECTION("when parsing a compound footnote", "[compound]") {
        
    RootElement *root =
        orgcpp::parse("./resources/12-footnote/02-compound-footnote.org");
    Headline *headline = root->get<Headline>(0);
    REQUIRE(headline);    
    Footnote *footnote = headline->get<Footnote>(0);
    REQUIRE(footnote);   
  }  
}
