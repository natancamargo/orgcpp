#include <catch2/catch_test_macros.hpp>

#include "orgcpp/orgcpp.h"

using namespace orgcpp;

TEST_CASE("drawer parsing should work properly", "[orgcpp][drawer]") {
  SECTION("when parsing a basic drawer", "[basic]") {
    RootElement *root =
        orgcpp::parse("./resources/02-drawer/01-basic-drawer.org");
    Drawer *drawer = root->get<Drawer>(0);
    REQUIRE(drawer);
    std::string head = drawer->getAttribute<Region>("head")->string();
    REQUIRE(head == "DRAWER");
    std::string body = drawer->getAttribute<Region>("body")->string();
    REQUIRE(body == "Text");
  }
  SECTION("when parsing a headline plus a drawer", "[headline]") {
    RootElement *root =
        orgcpp::parse("./resources/02-drawer/02-headline-drawer.org");
    Headline *headline = root->get<Headline>(0);
    REQUIRE(headline);
    Drawer *drawer = headline->get<Drawer>(1);
    REQUIRE(drawer);
    std::string head = drawer->getAttribute<Region>("head")->string();
    REQUIRE(head == "DRAWER");
    std::string body = drawer->getAttribute<Region>("body")->string();
    REQUIRE(body == "Text 2");
  }
  SECTION("when parsing a listitem plus a drawer", "[listitem]") {
    RootElement *root =
        orgcpp::parse("./resources/02-drawer/03-listitem-drawer.org");
    Listitem *listitem = root->get<Listitem>(0);
    REQUIRE(listitem);
    Drawer *drawer = listitem->get<Drawer>(1);
    REQUIRE(drawer);
    std::string head = drawer->getAttribute<Region>("head")->string();
    REQUIRE(head == "DRAWER");
    std::string body = drawer->getAttribute<Region>("body")->string();
    REQUIRE(body == "  Text 2");
  }
  SECTION("when parsing a nested drawer", "[nested]") {
    RootElement *root =
        orgcpp::parse("./resources/02-drawer/04-nested-drawer.org");
    Drawer *drawer1 = root->get<Drawer>(0);    
    REQUIRE(drawer1);
    Drawer *drawer2 = drawer1->get<Drawer>(0);
    REQUIRE(drawer2);
    std::string head = drawer2->getAttribute<Region>("head")->string();
    REQUIRE(head == "DRAWER_2");
    std::string body = drawer2->getAttribute<Region>("body")->string();
    REQUIRE(body == "Text");
  }
  SECTION("when parsing a compound drawer", "[compound]") {
    RootElement *root =
        orgcpp::parse("./resources/02-drawer/05-compound-drawer.org");
    Drawer *drawer = root->get<Drawer>(0);    
    REQUIRE(drawer);
    Listitem *listitem = drawer->get<Listitem>(0);
    REQUIRE(listitem);
    Text *text = listitem->get<Text>(0);
    REQUIRE(text);    
  }  
}
