#pragma once

#include "element/root.h"
#include <cstddef>
#include <fstream>
#include <memory>
#include <vector>
namespace orgcpp {
enum class ParserResultType {
  NONE,
  PARTIAL,
  FULL,
};
template <typename T> using ParserResult = std::tuple<ParserResultType, T *>;
template <typename T> class Parser {
public:
  virtual void parse(RootElement *root, const char *buffer,
                     const std::size_t size, std::size_t cursor) = 0;
  const std::vector<ParserResult<T>> &getResults() const;

protected:
  std::vector<ParserResult<T>> _results;
};
} // namespace orgcpp
