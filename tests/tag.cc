#include <catch2/catch_test_macros.hpp>

#include "orgcpp/orgcpp.h"

using namespace orgcpp;

TEST_CASE("tag parsing should work properly", "[orgcpp][tag]") {
  SECTION("when parsing a basic tag", "[basic]") {
    RootElement *root =
        orgcpp::parse("./resources/08-tag/01-basic-tag.org");
    Headline *headline = root->get<Headline>(0);
    REQUIRE(headline);    
    Tag *tag1 = headline->get<Tag>(0);
    REQUIRE(tag1);
    std::string head1 = tag1->getAttribute<Region>("head")->string();
    REQUIRE(head1 == "tag0");
  }
  SECTION("when parsing multiple tag", "[multiple]") {
    RootElement *root =
        orgcpp::parse("./resources/08-tag/02-multiple-tag.org");
    Headline *headline = root->get<Headline>(0);
    REQUIRE(headline);    
    Tag *tag1 = headline->get<Tag>(0);
    REQUIRE(tag1);
    std::string head1 = tag1->getAttribute<Region>("head")->string();
    REQUIRE(head1 == "tag0");
    Tag *tag2 = headline->get<Tag>(1);
    REQUIRE(tag2);
    std::string head2 = tag2->getAttribute<Region>("head")->string();
    REQUIRE(head2 == "tag1");
  }
  SECTION("when parsing compound tag", "[compound]") {
    RootElement *root =
        orgcpp::parse("./resources/08-tag/03-compound-tag.org");
    Headline *headline1 = root->get<Headline>(0);
    REQUIRE(headline1);
    Headline *headline2 = headline1->get<Headline>(2);
    REQUIRE(headline2);      
    Tag *tag1 = headline2->get<Tag>(0);
    REQUIRE(tag1);
  }  
}
