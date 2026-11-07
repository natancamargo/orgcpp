#pragma once

#include "element/link.h"
#include "element/root.h"
#include "parser/parser.h"
namespace orgcpp {
class LinkParser : public Parser<Link> {
public:
  void parse(RootElement *root, const char *buffer, const std::size_t size,
             std::size_t cursor) override;
};
} // namespace orgcpp
