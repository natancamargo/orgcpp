#pragma once

#include "element/listitem.h"
#include "element/root.h"
#include "parser/parser.h"
namespace orgcpp {
class ListitemParser : public Parser<Listitem> {
public:
  void parse(RootElement *root, const char *buffer, const std::size_t size,
             std::size_t cursor) override;
};
} // namespace orgcpp
