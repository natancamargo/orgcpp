
#include <catch2/catch_test_macros.hpp>

#include "orgcpp/orgcpp.h"

using namespace orgcpp;

TEST_CASE("link parsing should work properly", "[orgcpp][link]") {
  SECTION("when parsing a basic link", "[basic]") {
    RootElement *root = orgcpp::parse("./resources/06-link/01-basic-link.org");
    Link *link = root->get<Link>(0);
    REQUIRE(link);
    std::string head = link->getAttribute<Region>("head")->string();
    REQUIRE(head == "link");
    std::string content = link->getAttribute<Region>("content")->string();
    REQUIRE(content == "[[link]]");
  }
}
