
#include "matcher/headline-matcher.h"
#include "element/element.h"
#include "element/headline.h"
#include <cstddef>
#include <iostream>
#include <ostream>
#include <regex>

namespace orgcpp {
HeadlineMatcher::HeadlineMatcher() {
  _regex = "(^\\*+)\\s+([^\n]+)(\n(.+))?";
}
  void HeadlineMatcher::match(std::shared_ptr<std::ifstream> in, const std::string &buffer, std::size_t cursor) {
  std::sregex_iterator sregexBegin =
      std::sregex_iterator(buffer.begin(), buffer.end(), _regex);
  std::sregex_iterator sregexEnd = std::sregex_iterator();
  bool hasMatchs = std::distance(sregexBegin, sregexEnd) > 0;
  for (std::sregex_iterator i = sregexBegin; i != sregexEnd; ++i) {
    std::smatch match = *i;
    std::string match_str = match.str();
    std::size_t innerCursor = cursor + match.position();

    // Empty headline
    Headline headline;

    // first group (* asterisks)
    std::sub_match subMatch1 = match[1];
    //std::cout << subMatch.str() << std::endl;
    Region region1 = {innerCursor, innerCursor + subMatch1.length()};
    headline.setAttribute("before", region1);
    headline.setAttribute("level", static_cast<int>(subMatch1.length()));
    innerCursor += subMatch1.length();

    // second group (* title)
    std::sub_match subMatch2 = match[2];
    std::cout << subMatch2.str() << std::endl;
    Region region2 = {innerCursor, innerCursor + subMatch2.length()};
    headline.setAttribute("head", region2);
    innerCursor += subMatch2.length();
  }
}
} // namespace orgcpp
