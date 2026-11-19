#include <catch2/catch_test_macros.hpp>

#include "orgcpp/orgcpp.h"

using namespace orgcpp;

TEST_CASE("latex parsing should work properly", "[orgcpp][latex]") {
  SECTION("when parsing a basic latex", "[basic]") {
    RootElement *root =
        orgcpp::parse("./resources/11-latex/01-basic-latex.org");
    Latex *latex = root->get<Latex>(0);
    REQUIRE(latex);
    std::string head = latex->getAttribute<Region>("head")->string();
    REQUIRE(head == "equation");
    std::string body = latex->getAttribute<Region>("body")->string();
    REQUIRE(body == "latex");
  }
  SECTION("when parsing a compound latex", "[compound]") {
    RootElement *root =
        orgcpp::parse("./resources/11-latex/02-compound-latex.org");
    Headline *headline = root->get<Headline>(0);
    REQUIRE(headline);    
    Latex *latex1 = headline->get<Latex>(0);
    REQUIRE(latex1);

    Listitem *listitem = headline->get<Listitem>(1);
    REQUIRE(listitem);
    Latex *latex2 = listitem->get<Latex>(0);
    REQUIRE(latex2);    
  }  
}
