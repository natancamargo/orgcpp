#pragma once

#include <string>
#include "config/config.h"

#include "element/element.h"
#include "element/elements.h"
#include "element/root-element.h"
#include "element/headline.h"

namespace orgcpp {
  RootElement parse(std::string path);
}
