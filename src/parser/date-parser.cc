#include "parser/date-parser.h"
#include "allocator/allocator-impl.h"
#include "allocator/allocator.h"
#include "element/root.h"
#include "parser/parser.h"
#include "util/log.h"
#include "util/parsing.h"
#include <cstddef>
namespace orgcpp {
void DateParser::parse(RootElement *root, const char *buffer,
                       const std::size_t size, std::size_t cursor) {
  _results.clear();
  char lastCharacter = 0x00;
  bool begin = false;
  bool beginLine = false;
  bool beginFile = false;
  for (std::size_t i = 0; i < size; i++) {
    beginFile = i == 0;
    beginLine = lastCharacter == '\n' || beginFile;

    begin = beginLine && isBeginDate(root, buffer, size, i, cursor);

    if (begin) {
      Region region{root->getDocument()};
      region.first = i + cursor;
      region.second = getLineBreakIndex(buffer, size, i) + cursor;
      Date *date = allocator::allocate<Date>();
      date->setAttribute("content", region);
      ParserResultType type = ParserResultType::FULL;

      populateDate(date, root, buffer, size, cursor);
      ParserResult<Date> result{type, date};
      _results.push_back(result);
      i = region.second;

      // logs
      log() << "xxxxxxxxx date xxxxxxxxx" << std::endl;
      if (Region *preRegion = date->getAttribute<Region>("pre")) {
        log() << "------- pre -------" << std::endl;
        log() << preRegion->string() << std::endl;
        log() << "-------------------" << std::endl;
      }
      log() << "======= head =======" << std::endl;
      log() << date->getAttribute<Region>("head")->string() << std::endl;
      log() << "====================" << std::endl;
      log() << "xxxxxxxxxxxxxxxxxxxxxxxx" << std::endl;
    }

    lastCharacter = buffer[i];
  }
}
void populateDate(Date *date, RootElement *root, const char *buffer,
                  std::size_t size, std::size_t cursor) {
  Region *region = date->getAttribute<Region>("content");
  Region headRegion = getDateValue(root, buffer, size, region->first, cursor);
  date->setAttribute("head", headRegion);
  Region preRegion = getDatePre(root, buffer, size, region->first, cursor);
  if (preRegion.valid()) {
    date->setAttribute("pre", preRegion);
  }
}
} // namespace orgcpp
