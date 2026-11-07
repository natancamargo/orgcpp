#pragma once

#include "parser/headline-parser.h"
#include "parser/link-parser.h"
#include "parser/listitem-parser.h"
#include "parser/parser-impl.h"
#include "parser/parser.h"
namespace orgcpp {
class ParserManager {
public:
  std::vector<Element *>
  treefyResults(std::vector<ParserResult<Element>> treeFreeResults);
  template <typename T>
  void collectResults(Parser<T> &parser,
                      std::vector<ParserResult<Element>> &results);
  void parse(RootElement *root, const char *buffer, const std::size_t size,
             std::size_t cursor);

private:
  HeadlineParser _headlineParser;
  ListitemParser _listitemParser;
  LinkParser _linkParser;
};
} // namespace orgcpp
