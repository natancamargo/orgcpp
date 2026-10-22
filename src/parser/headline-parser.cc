#include "parser/headline-parser.h"
namespace orgcpp {
HeadlineParser::HeadlineParser() { _regex = "(\\*+) ([^\n]+)(\n.+)?"; }
} // namespace orgcpp
