#include <catch2/catch_test_macros.hpp>
#include <vector>

#include "orgcpp/orgcpp.h"

using namespace orgcpp;

TEST_CASE("find should work properly", "[orgcpp][findOneByType]") {
  SECTION("when finding one element", "[findOne]") {
    RootElement *root =
        orgcpp::parse("./resources/06-listitem/06-headline-listitem.org");
    Listitem *listitem = dynamic_cast<Listitem *>(root->findOne(
        [](Element *element) { return element->getType() == Type::LISTITEM; }));
    REQUIRE(listitem);
  }
  SECTION("when finding multiple element", "[findAll]") {
    RootElement *root =
        orgcpp::parse("./resources/06-listitem/06-headline-listitem.org");
    std::vector<Element *> listitems = root->findAll([](Element *element) {
          return element->getType() == Type::LISTITEM;
        });
    REQUIRE(listitems.size() == 4);
  }
}
