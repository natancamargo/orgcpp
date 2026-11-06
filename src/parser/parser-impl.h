#pragma once

#include "parser/parser.h"
#include <vector>
namespace orgcpp {
template <typename T>
const std::vector<ParserResult<T>> &Parser<T>::getResults() const {
  return _results;
}
} // namespace orgcpp
