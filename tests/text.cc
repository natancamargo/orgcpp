
#include <catch2/catch_test_macros.hpp>

#include "orgcpp/orgcpp.h"

using namespace orgcpp;

TEST_CASE("text parsing should work properly", "[orgcpp][text]") {
  SECTION("when parsing a basic text", "[basic]") {
    RootElement *root = orgcpp::parse("./resources/10-text/01-basic-text.org");
    Text *text = root->get<Text>(0);
    REQUIRE(text);
    std::string content = text->getAttribute<Region>("content")->string();
    std::string::difference_type lines = std::count(content.begin(), content.end(), '\n');
    REQUIRE(lines == 6);
  }
  SECTION("when parsing a headline text", "[headline]") {
    RootElement *root = orgcpp::parse("./resources/10-text/02-headline-text.org");
    Headline *headline1 = root->get<Headline>(0);
    Text *text1 = headline1->get<Text>(0);
    Headline *headline2 = headline1->get<Headline>(1);
    Text *text2 = headline2->get<Text>(0);
    REQUIRE(text1);
    REQUIRE(text2);
    std::string content1 = text1->getAttribute<Region>("content")->string();
    REQUIRE(content1 == "Text 1\n");
    std::string content2 = text2->getAttribute<Region>("content")->string();
    REQUIRE(content2 == "Text 2\n");
  }
  SECTION("when parsing a listitem text", "[basic]") {
    RootElement *root = orgcpp::parse("./resources/10-text/03-listitem-text.org");
    Listitem *listitem1 = root->get<Listitem>(0);
    Text *text1 = listitem1->get<Text>(0);
    Listitem *listitem2 = listitem1->get<Listitem>(1);
    Text *text2 = listitem2->get<Text>(0);
    Text *text3 = listitem1->get<Text>(2);
    REQUIRE(text1);
    REQUIRE(text2);
    REQUIRE(text3);
    std::string content1 = text1->getAttribute<Region>("content")->string();
    REQUIRE(content1 == "  Text 1\n");
    std::string content2 = text2->getAttribute<Region>("content")->string();
    REQUIRE(content2 == "    Text 2\n");
    std::string content3 = text3->getAttribute<Region>("content")->string();
    REQUIRE(content3 == "  Text 3\n");    
  }  
}
