#pragma once

#include "element/element.h"
#include "element/element-impl.h"
namespace orgcpp {
class Listitem : public Element {
public:
  Listitem();  
  bool isCheckbox();
};
} // namespace orgcpp
