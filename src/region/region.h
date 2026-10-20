#pragma once

#include <fstream>
#include <string>
#include <utility>
namespace orgcpp {
class Region : public std::pair<std::size_t, std::size_t> {
public:
  const std::string read(std::ifstream *document) const;
};
} // namespace orgcpp
