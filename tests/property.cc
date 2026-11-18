#include <catch2/catch_test_macros.hpp>

#include "orgcpp/orgcpp.h"

using namespace orgcpp;

TEST_CASE("property parsing should work properly", "[orgcpp][property]") {
  SECTION("when parsing a basic property", "[basic]") {
    RootElement *root =
        orgcpp::parse("./resources/07-properties/01-basic-properties.org");
    Drawer *drawer = root->get<Drawer>(0);
    REQUIRE(drawer);    
    Property *property1 = drawer->get<Property>(0);
    REQUIRE(property1);
    std::string head1 = property1->getAttribute<Region>("head")->string();
    REQUIRE(head1 == "KEY0");
    std::string body1 = property1->getAttribute<Region>("body")->string();
    REQUIRE(body1 == "VALUE0");
    Property *property2 = drawer->get<Property>(1);
    REQUIRE(property2);
    std::string head2 = property2->getAttribute<Region>("head")->string();
    REQUIRE(head2 == "KEY1");
    std::string body2 = property2->getAttribute<Region>("body")->string();
    REQUIRE(body2 == "VALUE1");
  }
  SECTION("when parsing a basic property", "[basic]") {
    RootElement *root =
        orgcpp::parse("./resources/07-properties/02-compound-properties.org");
    Headline *headline = root->get<Headline>(0);
    REQUIRE(headline);    
    Drawer *properties1 = headline->get<Drawer>(0);
    REQUIRE(properties1);
    Property *property1 = properties1->get<Property>(0);
    REQUIRE(property1);
    Property *property2 = properties1->get<Property>(1);
    REQUIRE(property2);

    Listitem *listitem = headline->get<Listitem>(1);
    REQUIRE(listitem);    
    Drawer *properties2 = listitem->get<Drawer>(0);
    REQUIRE(properties2);
    Property *property3 = properties2->get<Property>(0);
    REQUIRE(property3);        
  }  
}
