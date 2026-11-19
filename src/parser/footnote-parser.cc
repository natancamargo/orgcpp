#include "parser/footnote-parser.h"
#include "allocator/allocator-impl.h"
#include "allocator/allocator.h"
#include "element/root.h"
#include "parser/parser.h"
#include "util/log.h"
#include "util/parsing.h"
#include <cstddef>
namespace orgcpp {
void FootnoteParser::parse(RootElement *root, const char *buffer,
                       const std::size_t size, std::size_t cursor) {
  _results.clear();
  bool begin = false;
  for (std::size_t i = 0; i < size; i++) {
    begin = isBeginFootnote(root, buffer, size, i, cursor);

    if (begin) {
      Region region{root->getDocument()};
      region.first = i + cursor;
      Footnote *footnote = allocator::allocate<Footnote>();
      footnote->setAttribute("content", region);
      ParserResultType type = ParserResultType::FULL;

      populateFootnote(footnote, root, buffer, size, cursor);
      ParserResult<Footnote> result{type, footnote};
      _results.push_back(result);

      Region *headRegion = footnote->getAttribute<Region>("head");
      i = headRegion->second + 1;
      
      // logs
      log() << "xxxxxxxxx footnote xxxxxxxxx" << std::endl;
      log() << "======= head =======" << std::endl;
      log() << headRegion->string() << std::endl;
      log() << "====================" << std::endl;
      log() << "xxxxxxxxxxxxxxxxxxxxxxxx" << std::endl;
    }
  }
}
void populateFootnote(Footnote *footnote, RootElement *root, const char *buffer,
                  std::size_t size, std::size_t cursor) {
  Region *region = footnote->getAttribute<Region>("content");
  Region headRegion = getFootnoteValue(root, buffer, size, region->first, cursor);
  footnote->setAttribute("head", headRegion);
  region->second = headRegion.second + 1;  
}
} // namespace orgcpp
