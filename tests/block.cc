#include <catch2/catch_test_macros.hpp>

#include "element/element.h"
#include "orgcpp/orgcpp.h"

using namespace orgcpp;

TEST_CASE("block parsing should work properly", "[orgcpp][block]") {
  SECTION("when parsing a basic block", "[basic]") {
    RootElement *root =
        orgcpp::parse("./resources/03-block/01-basic-block.org");
    Block *block = root->get<Block>(0);
    REQUIRE(block);
    std::string head = block->getAttribute<Region>("head")->string();
    REQUIRE(head == "src");
    std::string body = block->getAttribute<Region>("body")->string();
    REQUIRE(body == "Text");
    std::string blockType = block->getAttribute<Region>("blockType")->string();
    REQUIRE(blockType == "c++");
    std::string content = block->getAttribute<Region>("content")->string();
    REQUIRE(content == "#+begin_src c++\nText\n#+end_src");
  }
  SECTION("when parsing variants of a block", "[variants]") {
    RootElement *root =
        orgcpp::parse("./resources/03-block/02-variants-block.org");
    Block *block1 = root->get<Block>(0);    
    REQUIRE(block1);
    std::string head1 = block1->getAttribute<Region>("head")->string();
    REQUIRE(head1 == "export");
    std::string body1 = block1->getAttribute<Region>("body")->string();
    REQUIRE(body1 == "ascii");
    std::string blockType1 = block1->getAttribute<Region>("blockType")->string();
    REQUIRE(blockType1 == "ascii");

    Block *block2 = root->get<Block>(2);    
    REQUIRE(block2);
    std::string head2 = block2->getAttribute<Region>("head")->string();
    REQUIRE(head2 == "example");
    std::string body2 = block2->getAttribute<Region>("body")->string();
    REQUIRE(body2 == "example");

    Block *block3 = root->get<Block>(3);    
    REQUIRE(block3);
    std::string head3 = block3->getAttribute<Region>("head")->string();
    REQUIRE(head3 == "custom");
    std::string body3 = block3->getAttribute<Region>("body")->string();
    REQUIRE(body3 == "custom");
  }
  SECTION("when parsing a compound block", "[compound]") {
    RootElement *root =
        orgcpp::parse("./resources/03-block/04-compound-block.org");
    Headline *headline = root->get<Headline>(0);
    REQUIRE(headline);    
    Block *block1 = headline->get<Block>(0);
    REQUIRE(block1);
    std::string head1 = block1->getAttribute<Region>("head")->string();
    REQUIRE(head1 == "export");
    std::string body1 = block1->getAttribute<Region>("body")->string();
    REQUIRE(body1 == "html");
    std::string blockType1 = block1->getAttribute<Region>("blockType")->string();
    REQUIRE(blockType1 == "html");

    Listitem *listitem = headline->get<Listitem>(1);
    REQUIRE(listitem);    
    Block *block2 = listitem->get<Block>(0);
    REQUIRE(block2);
    std::string head2 = block2->getAttribute<Region>("head")->string();
    REQUIRE(head2 == "quote");
    std::string body2 = block2->getAttribute<Region>("body")->string();
    REQUIRE(body2 == "  quote");    
  }  
}
