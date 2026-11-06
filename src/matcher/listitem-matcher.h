#pragma once

#include "element/listitem.h"
#include "element/root.h"
#include "matcher/matcher.h"
namespace orgcpp {
class ListitemMatcher : public Matcher<Listitem> {
public:
  void match(RootElement *root, const char *buffer, const std::size_t size,
             std::size_t cursor) override;
};
} // namespace orgcpp
