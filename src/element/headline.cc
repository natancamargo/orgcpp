#include "element/headline.h"
#include <optional>

namespace orgcpp {
bool Headline::isTodo() {
  return getAttribute<std::string>("todo").has_value();
}
} // namespace orgcpp
