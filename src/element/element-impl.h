#pragma once

#include "attribute/attribute.h"
#include "element/element.h"
namespace orgcpp {
template <typename T> std::optional<T> Element::getAttribute(const std::string &name) {
  try {
    Attribute attribute = _attributes.at(name);
    return std::get<T>(attribute.getValue());
  } catch (const std::out_of_range &e) {
    return std::nullopt;
  } catch (const std::bad_variant_access &ex) {
    return std::nullopt;
  }
}
} // namespace orgcpp
