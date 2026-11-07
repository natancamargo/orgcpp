
#include "element/element.h"
#include "region/region.h"
#include <cstddef>

namespace orgcpp {
void parseCheckbox(Element *element, const char *buffer, const std::size_t size,
                   const std::size_t cursor) {
  Region *headRegion = element->getAttribute<Region>("head");
  char lastCharacter = 0x00;
  bool begin = false;
  bool middle = false;
  bool end = false;
  bool lastMiddle = false;
  bool lastEnd = false;
  for (std::size_t i = (headRegion->first - cursor);
       i < (headRegion->second - cursor); i++) {

    begin = buffer[i] == '[' && lastCharacter != '[';
    middle = (lastMiddle && !lastEnd) || begin;
    end = (lastMiddle && buffer[i] == ']');

    if (end) {
      bool checked = buffer[i - 1] == 'X';
      element->setAttribute("checkbox", checked);
      if (i < size - 1 && buffer[i + 1] == ' ') {
        headRegion->first = i + 2;
      } else {
        headRegion->first = i + 1;
      }
      break;
    }

    lastMiddle = middle;
    lastEnd = end;
    lastCharacter = buffer[i];
  }
}
} // namespace orgcpp
