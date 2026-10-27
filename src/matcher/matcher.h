#pragma once

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
  virtual void match(std::shared_ptr<std::ifstream> in,
                     const std::string &buffer, std::size_t cursor) = 0;
  const std::vector<Match<T>> &getMatches() const;

protected:
  std::vector<Match<T>> _matches;
};
} // namespace orgcpp
