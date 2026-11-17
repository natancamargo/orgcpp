#include "parser/block-parser.h"
#include "allocator/allocator-impl.h"
#include "allocator/allocator.h"
#include "element/root.h"
#include "parser/block-parser.h"
#include "parser/parser.h"
#include "util/log.h"
#include "util/parsing.h"
#include <cstddef>
#include <stack>
namespace orgcpp {
void BlockParser::parse(RootElement *root, const char *buffer,
                        const std::size_t size, std::size_t cursor) {
  _results.clear();
  char lastCharacter = 0x00;
  bool begin = false;
  bool end = false;
  bool beginLine = false;
  bool beginFile = false;
  std::map<std::string, std::stack<Block *>> blocks;
  for (std::size_t i = 0; i < size; i++) {
    beginFile = i == 0;
    beginLine = lastCharacter == '\n' || beginFile;

    begin = beginLine && isBeginBlock(root, buffer, size, i, cursor);
    end = beginLine && isEndBlock(root, buffer, size, i, cursor);

    if (begin) {
      Region region{root->getDocument()};
      region.first = i + cursor;
      Block *block = allocator::allocate<Block>();
      block->setAttribute("content", region);
      std::string blockName =
          getBlockBeginName(root, buffer, size, i, cursor).string();
      blocks[blockName].push(block);
    }

    if (end) {
      std::string blockName =
          getBlockEndName(root, buffer, size, i, cursor).string();
      std::stack blockStack = blocks[blockName];
      if (blockStack.size() >= 1) {
        Block *block = blockStack.top();
        blockStack.pop();
        Region *region = block->getAttribute<Region>("content");
        region->second = i + cursor;
        ParserResultType type = ParserResultType::FULL;

        populateBlock(block, root, buffer, size, cursor);
        ParserResult<Block> result{type, block};
        _results.push_back(result);
        
        // logs
        log() << "xxxxxxxxx block xxxxxxxxx" << std::endl;
        log() << "======= head =======" << std::endl;
        log() << block->getAttribute<Region>("head")->string() << std::endl;
        log() << "====================" << std::endl;
        if (block->getAttribute<Region>("blockType")) {
          log() << "******* type *******" << std::endl;
          log() << block->getAttribute<Region>("blockType")->string()
                << std::endl;
          log() << "********************" << std::endl;
        }
        log() << "~~~~~~~ body ~~~~~~~" << std::endl;
        log() << block->getAttribute<Region>("body")->string() << std::endl;
        log() << "~~~~~~~~~~~~~~~~~~~~" << std::endl;
        log() << "xxxxxxxxxxxxxxxxxxxxxxxx" << std::endl;
      }
    }

    lastCharacter = buffer[i];
  }
}
void populateBlock(Block *block, RootElement *root, const char *buffer,
                   std::size_t size, std::size_t cursor) {
  Region *region = block->getAttribute<Region>("content");
  Region headRegion = getBlockBeginName(root, buffer, region->second + 1,
                                        region->first, cursor);
  block->setAttribute("head", headRegion);
  Region typeRegion =
      getBlockType(root, buffer, region->second + 1, headRegion.second, cursor);
  if (typeRegion.valid()) {
    block->setAttribute("blockType", typeRegion);
  }
  Region bodyRegion{root->getDocument()};
  bodyRegion.first = getLineBreakIndex(buffer, size, region->first) + 1;
  bodyRegion.second = region->second - 1;
  block->setAttribute("body", bodyRegion);
  Region endRegion =
      getBlockEndName(root, buffer, size, region->second, cursor);
  region->second = endRegion.second; // update content
}
} // namespace orgcpp
