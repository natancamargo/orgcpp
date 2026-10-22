
#include "matcher/matcher.h"
namespace orgcpp {
template <typename T> const std::vector<T> &Matcher<T>::getMatches() const {
  return _matches;
}
template <typename T>
const std::vector<Region> &Matcher<T>::getMatchRegions() const {
  return _matchRegions;
}
template <typename T>
const std::vector<MatchType> &Matcher<T>::getMatchTypes() const {
  return _matchTypes;
}

} // namespace orgcpp
