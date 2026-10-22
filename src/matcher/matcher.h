#pragma once

#include "region/region.h"
#include <cstddef>
#include <fstream>
#include <regex>
#include <vector>
namespace orgcpp {
enum class MatchType {
  NONE,
  PARTIAL,
  FULL,
};
template <typename T> class Matcher {
public:
  /* const MatchType match(const std::string &buffer, std::size_t cursor, bool
   * eof); */
  /* std::size_t matchRegex(std::string buffer, std::regex regex); */
  virtual void match(std::shared_ptr<std::ifstream> in, const std::string &buffer, std::size_t cursor) = 0;
  const std::vector<T> &getMatches() const;
  const std::vector<Region> &getMatchRegions() const;
  const std::vector<MatchType> &getMatchTypes() const;

protected:
  std::vector<T> _matches;
  std::vector<Region> _matchRegions;
  std::vector<MatchType> _matchTypes;
  std::regex _regex;
};
} // namespace orgcpp
