#pragma once

#include "element/element.h"
#include "element/element-impl.h"
namespace orgcpp {
class Headline : public Element {
public:
  Headline();  
  bool isTodo();
  bool isCheckbox();;
};
} // namespace orgcpp
