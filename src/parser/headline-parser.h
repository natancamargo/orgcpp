#pragma once

#include "element/headline.h"
#include "element/root.h"
#include "parser/parser.h"
namespace orgcpp {
class HeadlineParser : public Parser<Headline> {
public:
  void parse(RootElement *root, const char *buffer, const std::size_t size,
             std::size_t cursor) override;
};
} // namespace orgcpp
