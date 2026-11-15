#include "element/headline.h"

namespace orgcpp {
Headline::Headline() { _type = Type::HEADLINE; }
bool Headline::isTodo() { return !!getAttribute<std::string>("todo"); }
bool Headline::isCheckbox() { return !!getAttribute<bool>("checkbox"); }
} // namespace orgcpp
