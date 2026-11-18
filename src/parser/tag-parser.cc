#include "parser/tag-parser.h"
#include "allocator/allocator-impl.h"
#include "allocator/allocator.h"
#include "element/root.h"
#include "parser/parser.h"
#include "util/log.h"
#include "util/parsing.h"
#include <cstddef>
namespace orgcpp {
void TagParser::parse(RootElement *root, const char *buffer,
                      const std::size_t size, std::size_t cursor) {
  _results.clear();
  char lastCharacter = 0x00;
  bool begin = false;
  bool beginLine = false;
  bool beginFile = false;
  bool headlineLine = false;
  for (std::size_t i = 0; i < size; i++) {
    beginFile = i == 0;
    beginLine = lastCharacter == '\n' || beginFile;
    headlineLine = beginLine ? isBeginHeadline(buffer, size, i) : headlineLine;

    begin = headlineLine && isBeginTag(root, buffer, size, i, cursor);

    if (begin) {
      Region region{root->getDocument()};
      region.first = getIndexAfterSpaces(buffer, size, i) + cursor;
      Tag *tag = allocator::allocate<Tag>();
      tag->setAttribute("content", region);
      ParserResultType type = ParserResultType::FULL;

      populateTag(tag, root, buffer, size, cursor);
      ParserResult<Tag> result{type, tag};
      _results.push_back(result);
      Region *headRegion = tag->getAttribute<Region>("head");
      i = headRegion->second - 1;

      // logs
      log() << "xxxxxxxxx tag xxxxxxxxx" << std::endl;
      log() << "======= head =======" << std::endl;
      log() << headRegion->string() << std::endl;
      log() << "====================" << std::endl;
      log() << "xxxxxxxxxxxxxxxxxxxxxxxx" << std::endl;
    }

    lastCharacter = buffer[i];
  }
}
void populateTag(Tag *tag, RootElement *root, const char *buffer,
                 std::size_t size, std::size_t cursor) {
  Region *region = tag->getAttribute<Region>("content");
  Region headRegion = getTagName(root, buffer, size, region->first, cursor);
  tag->setAttribute("head", headRegion);
  region->second = headRegion.second;
}
} // namespace orgcpp
