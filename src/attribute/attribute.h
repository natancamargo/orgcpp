#pragma once

#include "region/region.h"
#include <string>
#include <variant>
namespace orgcpp {
using AttributeValue =
    std::variant<Region, std::string, int, float>;
class Attribute {
public:
  Attribute();
  const std::string &getName() const;
  void setName(const std::string &name);

  const AttributeValue &getValue() const;
  void setValue(const AttributeValue &value);

private:
  std::string _name;
  AttributeValue _value;
};
} // namespace orgcpp
