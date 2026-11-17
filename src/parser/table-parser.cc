#include "parser/table-parser.h"
#include "allocator/allocator-impl.h"
#include "allocator/allocator.h"
#include "element/root.h"
#include "parser/parser.h"
#include "util/log.h"
#include "util/parsing.h"
#include <cstddef>
#include <stack>
namespace orgcpp {
void TableParser::parse(RootElement *root, const char *buffer,
                        const std::size_t size, std::size_t cursor) {
  _results.clear();
  char lastCharacter = 0x00;
  bool begin = false;
  bool end = false;
  bool beginLine = false;
  bool beginFile = false;
  Table *table = nullptr;
  for (std::size_t i = 0; i < size; i++) {
    beginFile = i == 0;
    beginLine = lastCharacter == '\n' || beginFile;

    begin = beginLine && isBeginTable(buffer, size, i);
    end = beginLine && isEndTable(buffer, size, i);

    if (begin && !table) {
      Region region{root->getDocument()};
      region.first = i + cursor;
      table = allocator::allocate<Table>();
      table->setAttribute("content", region);
    }

    if (end && table) {
      Region *region = table->getAttribute<Region>("content");
      if (isFinalLine(buffer, size, i)) {
        region->second = size - 1;
      } else {
        region->second = i - 1;
      }
      populateTable(table, root, buffer, size, cursor);
      ParserResultType type = ParserResultType::FULL;
      ParserResult<Table> result{type, table};
      _results.push_back(result);
      i = region->second;

      // logs
      log() << "xxxxxxxxx table xxxxxxxxx" << std::endl;
      log() << "$$$$$$$ content $$$$$$$" << std::endl;
      log() << table->getAttribute<Region>("content")->string() << std::endl;
      log() << "$$$$$$$$$$$$$$$$$$$$$$$" << std::endl;
      log() << "xxxxxxxxxxxxxxxxxxxxxxxx" << std::endl;
      table = nullptr;
    }

    lastCharacter = buffer[i];
  }
}
void populateTable(Table *table, RootElement *root, const char *buffer,
                   std::size_t size, std::size_t cursor) {
  Region *region = table->getAttribute<Region>("content");
  Region headRegion{region->getDocument()};
  headRegion.first = region->first;
  headRegion.second = region->first + ((region->second - region->first) >= 12
                                           ? 12
                                           : (region->second - region->first));
  table->setAttribute("head", headRegion);
}
} // namespace orgcpp
