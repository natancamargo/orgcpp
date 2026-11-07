#include <catch2/catch_test_macros.hpp>

#include "orgcpp/orgcpp.h"

using namespace orgcpp;

TEST_CASE("listitem parsing should work properly", "[orgcpp][listitem]") {
  SECTION("when parsing a basic listitem", "[basic]") {
    RootElement *root =
        orgcpp::parse("./resources/07-listitem/01-basic-listitem.org");
    Listitem *listitem = root->get<Listitem>(0);
    REQUIRE(listitem);
    std::string head = listitem->getAttribute<Region>("head")->string();
    REQUIRE(head == "Listitem 1");
    std::string body = listitem->getAttribute<Region>("body")->string();
    REQUIRE(body == "  Text 1\n");
    std::string content = listitem->getAttribute<Region>("content")->string();
    REQUIRE(content == "- Listitem 1\n  Text 1\n");
    int *level = listitem->getAttribute<int>("level");
    REQUIRE(*level == 1);
  }
  SECTION("when parsing a nested listitem", "[nested]") {
    RootElement *root =
        orgcpp::parse("./resources/07-listitem/02-nested-listitem.org");
    Listitem *listitem1 = root->get<Listitem>(0);
    Listitem *listitem2 = listitem1->get<Listitem>(0);
    Listitem *listitem3 = listitem2->get<Listitem>(0);
    REQUIRE(listitem1);
    REQUIRE(listitem2);
    REQUIRE(listitem3);
    std::string head = listitem3->getAttribute<Region>("head")->string();
    REQUIRE(head == "Listitem 3");
    std::string body = listitem3->getAttribute<Region>("body")->string();
    REQUIRE(body == "      Text 1\n");
    std::string content = listitem3->getAttribute<Region>("content")->string();
    REQUIRE(content == "- Listitem 3\n      Text 1\n");
    int *level = listitem3->getAttribute<int>("level");
    REQUIRE(*level == 3);
  }
  SECTION("when parsing a head-only listitem", "[head-only]") {
    RootElement *root =
        orgcpp::parse("./resources/07-listitem/03-head-only-listitem.org");
    Listitem *listitem = root->get<Listitem>(0);
    REQUIRE(listitem);
    std::string head = listitem->getAttribute<Region>("head")->string();
    REQUIRE(head == "Listitem 1");
    int *level = listitem->getAttribute<int>("level");
    REQUIRE(*level == 1);
  }
  SECTION("when parsing a compound listitem", "[compound]") {
    RootElement *root =
        orgcpp::parse("./resources/07-listitem/04-compound-listitem.org");
    Listitem *listitem1 = root->get<Listitem>(0);
    Listitem *listitem2 = listitem1->get<Listitem>(0);
    Listitem *listitem3 = root->get<Listitem>(1);
    Listitem *listitem4 = root->get<Listitem>(2);

    REQUIRE(listitem1);
    std::string head1 = listitem1->getAttribute<Region>("head")->string();
    REQUIRE(head1 == "Listitem 1");
    std::string body1 = listitem1->getAttribute<Region>("body")->string();
    REQUIRE(body1 == "  Text 1\n  - Listitem 2\n    Text 2\n\n");
    std::string content1 = listitem1->getAttribute<Region>("content")->string();
    REQUIRE(content1 ==
            "- Listitem 1\n  Text 1\n  - Listitem 2\n    Text 2\n\n");
    int *level1 = listitem1->getAttribute<int>("level");
    REQUIRE(*level1 == 1);

    REQUIRE(listitem2);
    std::string head2 = listitem2->getAttribute<Region>("head")->string();
    REQUIRE(head2 == "Listitem 2");
    std::string body2 = listitem2->getAttribute<Region>("body")->string();
    REQUIRE(body2 == "    Text 2\n\n");
    std::string content2 = listitem2->getAttribute<Region>("content")->string();
    REQUIRE(content2 == "- Listitem 2\n    Text 2\n\n");
    int *level2 = listitem2->getAttribute<int>("level");
    REQUIRE(*level2 == 2);

    REQUIRE(listitem3);
    std::string head3 = listitem3->getAttribute<Region>("head")->string();
    REQUIRE(head3 == "Listitem 3");
    std::string body3 = listitem3->getAttribute<Region>("body")->string();
    REQUIRE(body3 == "  Text 3\n");
    std::string content3 = listitem3->getAttribute<Region>("content")->string();
    REQUIRE(content3 == "- Listitem 3\n  Text 3\n");
    int *level3 = listitem3->getAttribute<int>("level");
    REQUIRE(*level3 == 1);

    REQUIRE(listitem4 == nullptr);
  }
  SECTION("when parsing a listitem with a headline sibling",
          "[sibling-headline]") {
    RootElement *root = orgcpp::parse(
        "./resources/07-listitem/05-sibling-headline-listitem.org");
    Listitem *listitem = root->get<Listitem>(0);
    REQUIRE(listitem);
    std::string head = listitem->getAttribute<Region>("head")->string();
    REQUIRE(head == "Listitem 1");
    Region *body = listitem->getAttribute<Region>("body");
    if (body) {
      REQUIRE(body->string() == "");
    }
    REQUIRE(body == nullptr);
    int *level = listitem->getAttribute<int>("level");
    REQUIRE(*level == 1);
  }
  SECTION("when parsing headlines and listitem", "[headline-and-listitem]") {
    RootElement *root =
        orgcpp::parse("./resources/07-listitem/06-headline-listitem.org");
    Headline *headline1 = root->get<Headline>(0);
    Listitem *listitem1 = headline1->get<Listitem>(0);
    Headline *headline2 = headline1->get<Headline>(1);
    Listitem *listitem2 = headline2->get<Listitem>(0);
    Listitem *listitem3 = listitem2->get<Listitem>(0);
    Headline *headline3 = root->get<Headline>(1);
    Listitem *listitem4 = headline3->get<Listitem>(0);
    REQUIRE(headline1);
    REQUIRE(headline2);
    REQUIRE(headline3);
    REQUIRE(listitem1);
    REQUIRE(listitem2);
    REQUIRE(listitem3);
    REQUIRE(listitem4);
  }
  SECTION("when parsing a checkbox listitem", "[checkbox]") {
    RootElement *root =
      orgcpp::parse("./resources/07-listitem/07-checkbox-listitem.org");
    Listitem *listitem1 = root->get<Listitem>(0);
    Listitem *listitem2 = root->get<Listitem>(1);
    
    REQUIRE(listitem1);
    std::string head1 =
      listitem1->getAttribute<Region>("head")->string();
    REQUIRE(head1 == "Listitem 1");
    REQUIRE(listitem1->isCheckbox());
    bool *checkbox1 =
      listitem1->getAttribute<bool>("checkbox");
    REQUIRE(*checkbox1 == false);

    REQUIRE(listitem2);
    std::string head2 =
      listitem2->getAttribute<Region>("head")->string();
    REQUIRE(head2 == "Listitem 2");
    REQUIRE(listitem2->isCheckbox());
    bool *checkbox2 =
      listitem2->getAttribute<bool>("checkbox");
    REQUIRE(*checkbox2 == true);
  }    
}
