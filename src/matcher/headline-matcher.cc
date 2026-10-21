
#include "matcher/headline-matcher.h"

namespace orgcpp {
  HeadlineMatcher::HeadlineMatcher() {
    _beginRegex = "^\\*+";
    _endRegex = "^\\*+";
  }
}
