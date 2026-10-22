#pragma once

#include "element/element.h"
#include <regex>
namespace orgcpp {
class Parser {
public:
  template <typename T> T parse(std::string buffer);
private:
  std::regex _templateRegex;
};
}
