#pragma once

#include "element/headline.h"
#include "parser/parser.h"
#include <regex>
namespace orgcpp {
class HeadlineParser : public Parser {
public:
  HeadlineParser();  
  Headline parse(std::string buffer);
private:
  std::regex _regex;
};
} // namespace orgcpp
