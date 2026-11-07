#pragma once

#include "element/element.h"
namespace orgcpp {
class Listitem : public Element {
public:
  bool isCheckbox();

protected:
  Type _type = Type::HEADLINE;
};
} // namespace orgcpp
