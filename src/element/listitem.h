#pragma once

#include "element/element.h"
namespace orgcpp {
class Listitem : public Element {
protected:
  Type _type = Type::HEADLINE;
};
} // namespace orgcpp
