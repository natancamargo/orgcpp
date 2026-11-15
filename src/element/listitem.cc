#include "element/listitem.h"

namespace orgcpp {
Listitem::Listitem() {
  _type = Type::LISTITEM;  
}
bool Listitem::isCheckbox() { return !!getAttribute<bool>("checkbox"); }
}
