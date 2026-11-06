#pragma once

#include "element/root.h"
#include <cstddef>
#include <fstream>
#include <memory>
#include <vector>
namespace orgcpp {
enum class MatchType {
  NONE,
  PARTIAL,
  FULL,
};
template <typename T> using Match = std::tuple<MatchType, T *>;
template <typename T> class Matcher {
public:
  virtual void match(RootElement *root, const char *buffer,
                     const std::size_t size, std::size_t cursor) = 0;
  const std::vector<Match<T>> &getMatches() const;

protected:
  std::vector<Match<T>> _matches;
};
} // namespace orgcpp
