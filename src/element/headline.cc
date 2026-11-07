#include "element/headline.h"
#include <optional>

namespace orgcpp {
bool Headline::isTodo() { return !!getAttribute<std::string>("todo"); }
bool Headline::isCheckbox() { return !!getAttribute<bool>("checkbox"); }
} // namespace orgcpp
