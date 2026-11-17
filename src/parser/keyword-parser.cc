#include "parser/keyword-parser.h"
#include "allocator/allocator-impl.h"
#include "allocator/allocator.h"
#include "element/root.h"
#include "parser/parser.h"
#include "util/log.h"
#include "util/parsing.h"
#include <cstddef>
namespace orgcpp {
void KeywordParser::parse(RootElement *root, const char *buffer,
                          const std::size_t size, std::size_t cursor) {
  _results.clear();
  char lastCharacter = 0x00;
  bool begin = false;
  bool beginLine = false;
  bool beginFile = false;
  for (std::size_t i = 0; i < size; i++) {
    beginFile = i == 0;
    beginLine = lastCharacter == '\n' || beginFile;

    begin = beginLine && isBeginKeyword(root, buffer, size, i, cursor);

    if (begin) {
      Region region{root->getDocument()};
      region.first = i + cursor;
      region.second = getLineBreakIndex(buffer, size, i) + cursor;
      Keyword *keyword = allocator::allocate<Keyword>();
      keyword->setAttribute("content", region);
      ParserResultType type = ParserResultType::FULL;

      populateKeyword(keyword, root, buffer, size, cursor);
      ParserResult<Keyword> result{type, keyword};
      _results.push_back(result);
      i = region.second;

      // logs
      log() << "xxxxxxxxx keyword xxxxxxxxx" << std::endl;
      log() << "======= head =======" << std::endl;
      log() << keyword->getAttribute<Region>("head")->string() << std::endl;
      log() << "====================" << std::endl;
      log() << "~~~~~~~ body ~~~~~~~" << std::endl;
      log() << keyword->getAttribute<Region>("body")->string() << std::endl;
      log() << "~~~~~~~~~~~~~~~~~~~~" << std::endl;
      log() << "xxxxxxxxxxxxxxxxxxxxxxxx" << std::endl;
    }

    lastCharacter = buffer[i];
  }
}
void populateKeyword(Keyword *keyword, RootElement *root, const char *buffer,
                     std::size_t size, std::size_t cursor) {
  Region *region = keyword->getAttribute<Region>("content");
  Region headRegion =
      getKeywordName(root, buffer, region->second + 1, region->first, cursor);
  keyword->setAttribute("head", headRegion);
  Region bodyRegion = getKeywordValue(root, buffer, region->second + 1,
                                      headRegion.second + 1, cursor);
  keyword->setAttribute("body", bodyRegion);
}
} // namespace orgcpp
