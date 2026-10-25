#pragma once

#include <algorithm>
#include <any>
#include <cstddef>
#include <map>
#include <memory>
#include <typeinfo>
#include <vector>
namespace orgcpp {
namespace allocator {
inline std::map<std::size_t, std::vector<std::any>> elementsMap;
template <class T> T *allocate() {
  std::shared_ptr<T> element = std::make_shared<T>();
  std::vector<std::any> &elements = elementsMap[typeid(T).hash_code()];
  elements.push_back(element);
  return element.get();
}
template <class T> void deallocate(T *element) {
  std::vector<std::any> &elements = elementsMap[typeid(T).hash_code()];
  std::vector<std::any>::iterator it = std::find_if(
      elements.begin(), elements.end(), [&element](std::any _elementAny) {
        std::shared_ptr<T> _element =
            std::any_cast<std::shared_ptr<T>>(_elementAny);
        return element == _element.get();
      });
  if (it != elements.end()) {
    elements.erase(it);
  }
}
} // namespace allocator
} // namespace orgcpp
