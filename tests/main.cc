#include <catch2/catch_test_macros.hpp>

#include "orgcpp/orgcpp.h"

using namespace orgcpp;

TEST_CASE("headline parsing should work properly", "[orgcpp][headline]") {
  SECTION("when parsing a basic headline", "[basic]") {
    RootElement root = orgcpp::parse("./resources/01-basic-headline.org");
    Headline *headline = root.getChildren()->get<Headline>(0);
    REQUIRE(headline);
    if (headline) {
      REQUIRE(headline->getHead() == "Headline");
      REQUIRE(headline->getBody() == "Text");
    }
  }
}
