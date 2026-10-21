
#include "matcher/matcher.h"
#include <cstddef>
#include <regex>
namespace orgcpp {
const MatchType Matcher::match(const std::string buffer, std::size_t cursor) {
  // Pattern match at the begin
  bool hasMatchedBeginRegex = false;
  std::size_t indexOfBeginRegex;
  if (_matchRegion.first >= 0) {
    hasMatchedBeginRegex = true;
    hasMatchedBeginRegex = _matchRegion.first;
  } else {
    indexOfBeginRegex = matchRegex(buffer, _beginRegex) + cursor;
    hasMatchedBeginRegex = indexOfBeginRegex >= 0;
    if (hasMatchedBeginRegex) {
      _matchRegion.first = cursor;
    } else {
      _matchRegion.first = -1;
    }
  }

  // Pattern match at the end
  std::size_t indexOfEndRegex = matchRegex(buffer, _endRegex);
  bool hasMatchedEndRegex = indexOfEndRegex >= 0;
  if (hasMatchedEndRegex) {
    _matchRegion.second = cursor;
  } else {
    _matchRegion.second = -1;
  }

  // Partial or Full match
  if (hasMatchedBeginRegex) {
    return MatchType::PARTIAL;
  } else if (hasMatchedBeginRegex && hasMatchedEndRegex &&
             (indexOfBeginRegex > indexOfEndRegex)) {
    return MatchType::FULL;
  } else if (hasMatchedBeginRegex) {
  }
  return MatchType::NONE;
}
std::size_t matchRegex(std::string buffer, std::regex regex) {
  std::sregex_iterator beginMatch =
      std::sregex_iterator(buffer.begin(), buffer.end(), regex);
  std::sregex_iterator endMatch = std::sregex_iterator();
  bool hasMatchs = std::distance(beginMatch, endMatch) > 0;
  return hasMatchs ? beginMatch->position() : -1;
}
const Region Matcher::getMatchRegion() const { return _matchRegion; }
} // namespace orgcpp
