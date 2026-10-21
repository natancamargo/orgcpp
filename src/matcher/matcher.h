#pragma once

#include "region/region.h"
#include <cstddef>
#include <regex>
namespace orgcpp {
enum class MatchType {
  NONE,
  PARTIAL,
  FULL,
};
class Matcher {
private:
  virtual ~Matcher() = 0;
  const MatchType match(const std::string buffer, std::size_t cursor);
  std::size_t matchRegex(std::string buffer, std::regex regex);
  const Region getMatchRegion() const;

public:
  Region _matchRegion;
  std::regex _beginRegex;
  std::regex _endRegex;
};
} // namespace orgcpp
