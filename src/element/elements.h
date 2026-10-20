#pragma once

#include "element/element.h"
#include <cstddef>
#include <optional>
#include <vector>

namespace orgcpp {
class Elements : public Element {
public:
  Element *get(std::size_t index) const;
  template <typename T> T *get(std::size_t index) const;

  const std::vector<Element*> &getElements() const;
  void addChild(Element *element);
  void removeChild(Element *element);  
private:
  std::vector<Element*> _elements;
};
} // namespace orgcpp
