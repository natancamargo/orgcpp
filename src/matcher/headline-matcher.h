#pragma once

#include "element/headline.h"
#include "matcher/matcher.h"
namespace orgcpp {
class HeadlineMatcher : public Matcher<Headline> {
public:
  void match(std::shared_ptr<std::ifstream> in, const std::string &buffer,
             std::size_t cursor) override;
};
} // namespace orgcpp
