#pragma once

#include "parser/drawer-parser.h"
#include "parser/headline-parser.h"
#include "parser/link-parser.h"
#include "parser/listitem-parser.h"
#include "parser/parser-impl.h"
#include "parser/parser.h"
#include "parser/text-parser.h"
namespace orgcpp {
class ParserManager {
public:
  void treefyResults(RootElement *root,
                     std::vector<ParserResult<Element>> treeFreeResults);
  template <typename T>
  void collectResults(Parser<T> &parser,
                      std::vector<ParserResult<Element>> &results);
  void parse(RootElement *root, const char *buffer, const std::size_t size,
             std::size_t cursor);

private:
  HeadlineParser _headlineParser;
  ListitemParser _listitemParser;
  LinkParser _linkParser;
  TextParser _textParser;
  DrawerParser _drawerParser;
};
std::string getHead(Element *element);
} // namespace orgcpp
