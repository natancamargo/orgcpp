#pragma once

#include "element/element.h"
#include "element/elements.h"

namespace orgcpp {
template <typename T> T *Elements ::get(std::size_t index) const {
  Element *element = get(index);
  return dynamic_cast<T *>(element);
}
} // namespace orgcpp
