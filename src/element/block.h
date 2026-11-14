#pragma once

#include "element/element-impl.h"
#include "element/element.h"
namespace orgcpp {
class Block : public Element {
protected:
  Type _type = Type::BLOCK;
};
} // namespace orgcpp
