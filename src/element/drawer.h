#pragma once

#include "element/element.h"
#include "element/element-impl.h"
namespace orgcpp {
class Drawer : public Element {
protected:
  Type _type = Type::DRAWER;
};
} // namespace orgcpp
