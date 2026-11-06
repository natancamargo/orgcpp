#pragma once

#include "parser/headline-parser.h"
#include "parser/listitem-parser.h"
#include "parser/parser.h"
#include "parser/parser-impl.h"
namespace orgcpp {
class ParserManager {
public:
  std::vector<Element *>
  treefyResults(std::vector<ParserResult<Element>> treeFreeResults);
  void parse(RootElement *root, const char *buffer, const std::size_t size,
             std::size_t cursor);
private:
  HeadlineParser _headlineParser;
  ListitemParser _listitemParser;
};
} // namespace orgcpp
