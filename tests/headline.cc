#include <catch2/catch_test_macros.hpp>

#include "orgcpp/orgcpp.h"

using namespace orgcpp;

TEST_CASE("headline parsing should work properly", "[orgcpp][headline]") {
  SECTION("when parsing a basic headline", "[basic]") {
    RootElement *root =
      orgcpp::parse("./resources/01-headline/01-basic-headline.org");
    Headline *headline = root->get<Headline>(0);
    REQUIRE(headline);
    std::string head = headline->getAttribute<Region>("head")->string();
    REQUIRE(head == "Headline");
    std::string body = headline->getAttribute<Region>("body")->string();
    REQUIRE(body == "Text\n");
    std::string content =
      headline->getAttribute<Region>("content")->string();
    REQUIRE(content == "* Headline\nText\n");
    int *level = headline->getAttribute<int>("level");
    REQUIRE(*level == 1);
  }
  SECTION("when parsing a compound headline", "[compound]") {
    RootElement *root =
        orgcpp::parse("./resources/01-headline/02-compound-headline.org");
    Headline *headline1 = root->get<Headline>(0);
    Headline *headline2 = root->get<Headline>(0)->get<Headline>(0);
    Headline *headline3 = root->get<Headline>(1);
    Headline *headline4 = root->get<Headline>(2);

    REQUIRE(headline1);
    std::string head1 =
      headline1->getAttribute<Region>("head")->string();
    REQUIRE(head1 == "Headline 1");
    std::string body1 =
      headline1->getAttribute<Region>("body")->string();
    REQUIRE(body1 == "Text 1\n** Headline 2\nText 2\n\n \n");
    int *level1 = headline1->getAttribute<int>("level");
    REQUIRE(*level1 == 1);
    std::string content1 =
      headline1->getAttribute<Region>("content")->string();
    REQUIRE(content1 == "* Headline 1\nText 1\n** Headline 2\nText 2\n\n \n");

    REQUIRE(headline2);
    std::string head2 =
      headline2->getAttribute<Region>("head")->string();
    REQUIRE(head2 == "Headline 2");
    std::string body2 =
      headline2->getAttribute<Region>("body")->string();
    REQUIRE(body2 == "Text 2\n\n \n");
    int *level2 = headline2->getAttribute<int>("level");
    REQUIRE(*level2 == 2);
    std::string content2 =
      headline2->getAttribute<Region>("content")->string();
    REQUIRE(content2 == "** Headline 2\nText 2\n\n \n");

    REQUIRE(headline3);
    std::string head3 =
      headline3->getAttribute<Region>("head")->string();
    REQUIRE(head3 == "Headline 3");
    std::string body3 =
      headline3->getAttribute<Region>("body")->string();
    REQUIRE(body3 == "Text 3\n");
    int *level3 = headline3->getAttribute<int>("level");
    REQUIRE(*level3 == 1);
    std::string content3 =
      headline3->getAttribute<Region>("content")->string();
    REQUIRE(content3 == "* Headline 3\nText 3\n");

    REQUIRE(headline4 == nullptr);
  }
  SECTION("when parsing a nested headline", "[nested]") {
    RootElement *root =
      orgcpp::parse("./resources/01-headline/03-nested-headline.org");
    Headline *headline1 = root->get<Headline>(0);
    Headline *headline2 = headline1->get<Headline>(0);
    Headline *headline3 = headline2->get<Headline>(0);

    REQUIRE(headline1);
    REQUIRE(headline2);
    REQUIRE(headline3);
    std::string head3 =
      headline3->getAttribute<Region>("head")->string();
    REQUIRE(head3 == "Headline 3");
    std::string body3 =
      headline3->getAttribute<Region>("body")->string();
    REQUIRE(body3 == "Text 3\n");
    int *level3 = headline3->getAttribute<int>("level");
    REQUIRE(*level3 == 3);
    std::string content3 =
      headline3->getAttribute<Region>("content")->string();
    REQUIRE(content3 == "*** Headline 3\nText 3\n");
  }
  SECTION("when parsing a nested basic-todo", "[todo]") {
    RootElement *root =
      orgcpp::parse("./resources/01-headline/04-basic-todo-headline.org");
    Headline *headline1 = root->get<Headline>(0);
    Headline *headline2 = root->get<Headline>(1);

    REQUIRE(headline1);
    std::string head1 =
      headline1->getAttribute<Region>("head")->string();
    REQUIRE(head1 == "Headline 1");
    REQUIRE(headline1->isTodo());
    std::string *todo1 =
      headline1->getAttribute<std::string>("todo");
    REQUIRE(*todo1 == "TODO");

    REQUIRE(headline2);
    std::string head2 =
      headline2->getAttribute<Region>("head")->string();
    REQUIRE(head2 == "Headline 2");
    REQUIRE(headline2->isTodo());
    std::string *todo2 =
      headline2->getAttribute<std::string>("todo");
    REQUIRE(*todo2 == "DONE");
  }
  SECTION("when parsing a checkbox headline", "[checkbox]") {
    RootElement *root =
      orgcpp::parse("./resources/01-headline/07-checkbox-headline.org");
    Headline *headline1 = root->get<Headline>(0);
    Headline *headline2 = root->get<Headline>(1);
    
    REQUIRE(headline1);
    std::string head1 =
      headline1->getAttribute<Region>("head")->string();
    REQUIRE(head1 == "Headline 1");
    REQUIRE(headline1->isCheckbox());
    bool *checkbox1 =
      headline1->getAttribute<bool>("checkbox");
    REQUIRE(*checkbox1 == false);

    REQUIRE(headline2);
    std::string head2 =
      headline2->getAttribute<Region>("head")->string();
    REQUIRE(head2 == "Headline 2");
    REQUIRE(headline2->isCheckbox());
    bool *checkbox2 =
      headline2->getAttribute<bool>("checkbox");
    REQUIRE(*checkbox2 == true);
  }
}
