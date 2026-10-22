#include "element/headline.h"
#include "element/elements.h"

namespace orgcpp {
Headline::Headline() { _children = new Elements(); }
Headline::~Headline() {
  delete _children;  
}
} // namespace orgcpp
