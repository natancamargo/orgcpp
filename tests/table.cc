#include <catch2/catch_test_macros.hpp>

#include "orgcpp/orgcpp.h"

using namespace orgcpp;

TEST_CASE("table parsing should work properly", "[orgcpp][table]") {
  SECTION("when parsing a basic table", "[basic]") {
    RootElement *root =
        orgcpp::parse("./resources/04-table/01-basic-table.org");
    Table *table = root->get<Table>(0);
    REQUIRE(table);
    std::string head = table->getAttribute<Region>("content")->string();
    REQUIRE(head == "| header 1 | header 2 |\n| cell 1   | cell 2   |");
  }
  SECTION("when parsing a basic table", "[compound]") {
    RootElement *root =
        orgcpp::parse("./resources/04-table/02-compound-table.org");
    Headline *headline = root->get<Headline>(0);
    REQUIRE(headline);
    Table *table1 = headline->get<Table>(0);
    REQUIRE(table1);
    Listitem *listitem = headline->get<Listitem>(1);
    REQUIRE(listitem);
    Table *table2 = listitem->get<Table>(0);
    REQUIRE(table2);
  }
}
