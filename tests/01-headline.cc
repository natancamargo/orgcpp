#include <catch2/catch_test_macros.hpp>

#include "orgcpp/orgcpp.h"

using namespace orgcpp;

TEST_CASE("headline parsing should work properly", "[orgcpp][headline]") {
  SECTION("when parsing a basic headline", "[basic]") {
    RootElement *root =
      orgcpp::parse("./resources/01-headline/01-basic-headline.org");
    Headline *headline = root->get<Headline>(0);
    REQUIRE(headline);
    std::string head = headline->getAttribute<Region>("head").value().string();
    REQUIRE(head == "Headline");
    std::string body = headline->getAttribute<Region>("body").value().string();
    REQUIRE(body == "Text\n");
    std::string content =
      headline->getAttribute<Region>("content").value().string();
    REQUIRE(content == "* Headline\nText\n");
    int level = headline->getAttribute<int>("level").value();
    REQUIRE(level == 1);
  }
  SECTION("when parsing a combined headline", "[combined]") {
    RootElement *root =
      orgcpp::parse("./resources/01-headline/02-combined-headline.org");
    Headline *headline1 = root->get<Headline>(0);
    Headline *headline2 = root->get<Headline>(0)->get<Headline>(0);
    Headline *headline3 = root->get<Headline>(1);
    Headline *headline4 = root->get<Headline>(2);

    REQUIRE(headline1);
    std::string head1 =
      headline1->getAttribute<Region>("head").value().string();
    REQUIRE(head1 == "Headline 1");
    std::string body1 =
      headline1->getAttribute<Region>("body").value().string();
    REQUIRE(body1 == "Text 1\n- ListItem 1\n** Headline 2\n- ListItem 2\n\n \n");
    int level1 = headline1->getAttribute<int>("level").value();
    REQUIRE(level1 == 1);
    std::string content1 =
      headline1->getAttribute<Region>("content").value().string();
    REQUIRE(content1 == "* Headline 1\nText 1\n- ListItem 1\n** Headline 2\n- ListItem 2\n\n \n");

    REQUIRE(headline2);
    std::string head2 =
      headline2->getAttribute<Region>("head").value().string();
    REQUIRE(head2 == "Headline 2");
    std::string body2 =
      headline2->getAttribute<Region>("body").value().string();
    REQUIRE(body2 == "- ListItem 2\n\n \n");
    int level2 = headline2->getAttribute<int>("level").value();
    REQUIRE(level2 == 2);
    std::string content2 =
      headline2->getAttribute<Region>("content").value().string();
    REQUIRE(content2 == "** Headline 2\n- ListItem 2\n\n \n");

    REQUIRE(headline3);
    std::string head3 =
      headline3->getAttribute<Region>("head").value().string();
    REQUIRE(head3 == "Headline 3");
    std::string body3 =
      headline3->getAttribute<Region>("body").value().string();
    REQUIRE(body3 == "Text 3\n");
    int level3 = headline3->getAttribute<int>("level").value();
    REQUIRE(level3 == 1);
    std::string content3 =
      headline3->getAttribute<Region>("content").value().string();
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
      headline3->getAttribute<Region>("head").value().string();
    REQUIRE(head3 == "Headline 3");
    std::string body3 =
      headline3->getAttribute<Region>("body").value().string();
    REQUIRE(body3 == "Text 3\n");
    int level3 = headline3->getAttribute<int>("level").value();
    REQUIRE(level3 == 3);
    std::string content3 =
      headline3->getAttribute<Region>("content").value().string();
    REQUIRE(content3 == "*** Headline 3\nText 3\n");
  }
  SECTION("when parsing a nested basic-todo", "[basic][todo]") {
    RootElement *root =
      orgcpp::parse("./resources/01-headline/04-basic-todo-headline.org");
    Headline *headline1 = root->get<Headline>(0);
    Headline *headline2 = root->get<Headline>(1);

    REQUIRE(headline1);
    std::string head1 =
      headline1->getAttribute<Region>("head").value().string();
    REQUIRE(head1 == "Headline 1");
    REQUIRE(headline1->isTodo());
    std::string todo1 =
      headline1->getAttribute<std::string>("todo").value();
    REQUIRE(todo1 == "TODO");

    REQUIRE(headline2);
    std::string head2 =
      headline2->getAttribute<Region>("head").value().string();
    REQUIRE(head2 == "Headline 2");
    REQUIRE(headline2->isTodo());
    std::string todo2 =
      headline2->getAttribute<std::string>("todo").value();
    REQUIRE(todo2 == "DONE");

  }
}
