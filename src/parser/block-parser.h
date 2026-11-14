#pragma once

#include "element/block.h"
#include "element/root.h"
#include "parser/parser.h"
namespace orgcpp {
class BlockParser : public Parser<Block> {
public:
  void parse(RootElement *root, const char *buffer, const std::size_t size,
             std::size_t cursor) override;
};
void populateBlock(Block *block, RootElement *root, const char *buffer,
                   std::size_t size, std::size_t cursor);
} // namespace orgcpp
