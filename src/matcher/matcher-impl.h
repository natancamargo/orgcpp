#pragma once

#include "matcher/matcher.h"
#include <vector>
namespace orgcpp {
template <typename T>
const std::vector<Match<T>> &Matcher<T>::getMatches() const {
  return _matches;
}
} // namespace orgcpp
