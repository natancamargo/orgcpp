#pragma once

#include "element/headline.h"
#include "element/root-element.h"
#include "matcher/matcher.h"
namespace orgcpp {
class HeadlineMatcher : public Matcher<Headline> {
public:
  void match(RootElement *root, const std::string &buffer,
             std::size_t cursor) override;
};
} // namespace orgcpp
