#pragma once

#include "element/keyword.h"
#include "element/root.h"
#include "parser/parser.h"
namespace orgcpp {
class KeywordParser : public Parser<Keyword> {
public:
  void parse(RootElement *root, const char *buffer, const std::size_t size,
             std::size_t cursor) override;
};
void populateKeyword(Keyword *keyword, RootElement *root, const char *buffer,
                    std::size_t size, std::size_t cursor);
} // namespace orgcpp
