#include <catch2/catch_test_macros.hpp>

#include "orgcpp/orgcpp.h"

using namespace orgcpp;

TEST_CASE("date parsing should work properly", "[orgcpp][date]") {
  SECTION("when parsing a basic date", "[basic]") {
    RootElement *root = orgcpp::parse("./resources/09-date/01-basic-date.org");
    Date *date = root->get<Date>(0);
    REQUIRE(date);
    std::string head = date->getAttribute<Region>("head")->string();
    REQUIRE(head == "2006-11-01 Wed 19:15");
  }
  SECTION("when parsing a deadline date", "[deadline]") {
    RootElement *root = orgcpp::parse("./resources/09-date/02-deadline-date.org");
    Date *date = root->get<Date>(0);
    REQUIRE(date);
    std::string head = date->getAttribute<Region>("head")->string();
    REQUIRE(head == "2006-11-01 Wed");
    std::string pre = date->getAttribute<Region>("pre")->string();
    REQUIRE(pre == "DEADLINE");
  }
  SECTION("when parsing a scheduled date", "[scheduled]") {
    RootElement *root = orgcpp::parse("./resources/09-date/03-scheduled-date.org");
    Date *date = root->get<Date>(0);
    REQUIRE(date);
    std::string head = date->getAttribute<Region>("head")->string();
    REQUIRE(head == "2006-11-01 Wed");
    std::string pre = date->getAttribute<Region>("pre")->string();
    REQUIRE(pre == "SCHEDULED");
  }
  SECTION("when parsing a compound date", "[compound]") {
    RootElement *root = orgcpp::parse("./resources/09-date/04-compound-date.org");
    Headline *headline1 = root->get<Headline>(0);
    REQUIRE(headline1);    
    Date *date1 = headline1->get<Date>(0);
    REQUIRE(date1);

    Headline *headline2 = root->get<Headline>(1);
    REQUIRE(headline2);    
    Date *date2 = headline2->get<Date>(0);
    REQUIRE(date2);

    Headline *headline3 = root->get<Headline>(1);
    REQUIRE(headline3);    
    Date *date3 = headline3->get<Date>(0);
    REQUIRE(date3);    
  }  
}
