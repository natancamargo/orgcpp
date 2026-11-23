#include "util/element.h"
#include <algorithm>

namespace orgcpp {
std::string getHead(Element *element) {
  std::string head;
  if (element->getAttribute<Region>("head")) {
    head = element->getAttribute<Region>("head")->string();
  } else if (element->getAttribute<std::string>("head")) {
    head = *element->getAttribute<std::string>("head");
  }
  head.erase(std::remove(head.begin(), head.end(), '\n'), head.end());
  return head;
}
} // namespace orgcpp
