#include "parser/property-parser.h"
#include "allocator/allocator-impl.h"
#include "allocator/allocator.h"
#include "element/root.h"
#include "parser/parser.h"
#include "util/log.h"
#include "util/parsing.h"
#include <cstddef>
namespace orgcpp {
void PropertyParser::parse(RootElement *root, const char *buffer,
                          const std::size_t size, std::size_t cursor) {
  _results.clear();
  char lastCharacter = 0x00;
  bool begin = false;
  bool beginLine = false;
  bool beginFile = false;
  for (std::size_t i = 0; i < size; i++) {
    beginFile = i == 0;
    beginLine = lastCharacter == '\n' || beginFile;

    begin = beginLine && isBeginProperty(root, buffer, size, i, cursor);

    if (begin) {
      Region region{root->getDocument()};
      region.first = i + cursor;
      region.second = getLineBreakIndex(buffer, size, i) + cursor;
      Property *property = allocator::allocate<Property>();
      property->setAttribute("content", region);
      ParserResultType type = ParserResultType::FULL;

      populateProperty(property, root, buffer, size, cursor);
      ParserResult<Property> result{type, property};
      _results.push_back(result);
      i = region.second;

      // logs
      log() << "xxxxxxxxx property xxxxxxxxx" << std::endl;
      log() << "======= head =======" << std::endl;
      log() << property->getAttribute<Region>("head")->string() << std::endl;
      log() << "====================" << std::endl;
      log() << "~~~~~~~ body ~~~~~~~" << std::endl;
      log() << property->getAttribute<Region>("body")->string() << std::endl;
      log() << "~~~~~~~~~~~~~~~~~~~~" << std::endl;
      log() << "xxxxxxxxxxxxxxxxxxxxxxxx" << std::endl;
    }

    lastCharacter = buffer[i];
  }
}
void populateProperty(Property *property, RootElement *root, const char *buffer,
                     std::size_t size, std::size_t cursor) {
  Region *region = property->getAttribute<Region>("content");
  Region headRegion =
      getPropertyName(root, buffer, region->second + 1, region->first, cursor);
  property->setAttribute("head", headRegion);
  Region bodyRegion = getPropertyValue(root, buffer, region->second + 1,
                                      headRegion.second + 1, cursor);
  property->setAttribute("body", bodyRegion);
}
} // namespace orgcpp
