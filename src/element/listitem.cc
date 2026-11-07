#include "element/listitem.h"

namespace orgcpp {
bool Listitem::isCheckbox() { return !!getAttribute<bool>("checkbox"); }
}
