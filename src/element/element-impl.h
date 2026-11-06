#pragma once

#include "attribute/attribute.h"
#include "element/element.h"
#include <iostream>
namespace orgcpp {
template <typename T>
T *Element::getAttribute(const std::string &name) {
  try {
    Attribute &attribute = _attributes.at(name);
    return std::get_if<T>(&attribute.getValue());
  } catch (const std::out_of_range &e) {
    return nullptr;
  }
}
template <typename T> T *Element::get(std::size_t index) const {
  Element *element = get(index);
  return dynamic_cast<T *>(element);
}
} // namespace orgcpp
