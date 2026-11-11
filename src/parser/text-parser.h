#pragma once

#include "element/text.h"
#include "element/root.h"
#include "parser/parser.h"
namespace orgcpp {
class TextParser : public Parser<Text> {
public:
  void parse(RootElement *root, const char *buffer, const std::size_t size,
             std::size_t cursor) override;
};
} // namespace orgcpp
