#include "element/root-element.h"
#include "element/elements.h"

namespace orgcpp {
RootElement::RootElement() { _children = new Elements(); }
RootElement::~RootElement() { delete _children; }
} // namespace orgcpp
