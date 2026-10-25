#pragma once

#include "region/region.h"
#include <cstddef>
#include <fstream>
#include <vector>
namespace orgcpp {
enum class MatchType {
  NONE,
  PARTIAL,
  FULL,
};
template <typename T> using Match = std::tuple<MatchType, Region, T *>;
template <typename T> class Matcher {
public:
  virtual void match(std::shared_ptr<std::ifstream> in,
                     const std::string &buffer, std::size_t cursor) = 0;
  const std::vector<Match<T>> &getMatches() const;

protected:
  std::vector<Match<T>> _matches;
};
} // namespace orgcpp
