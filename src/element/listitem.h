#pragma once

#include "element/element.h"
#include "element/element-impl.h"
namespace orgcpp {
class Listitem : public Element {
public:
  bool isCheckbox();

protected:
  Type _type = Type::HEADLINE;
};
} // namespace orgcpp
