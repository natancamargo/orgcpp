#include "element/elements.h"
#include <algorithm>
#include <optional>

namespace orgcpp {
Element *Elements::get(std::size_t index) const {
  try {
    return _elements.at(index);
  } catch (const std::out_of_range &e) {
    return nullptr;
  }
}
template <typename T> T *Elements::get(std::size_t index) const {
  Element *element = get(index);
  if (element) {
    return dynamic_cast<Element *>(element);
  }
  return std::nullopt;
}
const std::vector<Element *> &Elements::getElements() const {
  return _elements;
}

void Elements::addChild(Element *element) { _elements.push_back(element); }
void Elements::removeChild(Element *element) {
  _elements.erase(std::remove(_elements.begin(), _elements.end(), element));
}

} // namespace orgcpp
