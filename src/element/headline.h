#pragma once

#include "element/element.h"
namespace orgcpp {
class Headline : public Element {
public:
  bool isTodo();

protected:
  Type _type = Type::HEADLINE;
};
} // namespace orgcpp
