#include "parser/drawer-parser.h"
#include "allocator/allocator-impl.h"
#include "allocator/allocator.h"
#include "element/root.h"
#include "parser/parser.h"
#include "util/log.h"
#include "util/parsing.h"
#include <cstddef>
#include <stack>
namespace orgcpp {
void DrawerParser::parse(RootElement *root, const char *buffer,
                         const std::size_t size, std::size_t cursor) {
  _results.clear();
  char lastCharacter = 0x00;
  bool begin = false;
  bool end = false;
  bool beginLine = false;
  bool beginFile = false;
  std::stack<Drawer*> drawers; 
  for (std::size_t i = 0; i < size; i++) {
    beginFile = i == 0;
    beginLine = lastCharacter == '\n' || beginFile;

    // begin = beginLine && !lastMiddle &&
    //         isBeginDrawer(root, buffer, size, i, cursor);
    // middle = (lastMiddle && !lastEnd) || begin;
    // end = (lastMiddle && !lastEnd && beginLine &&
    //        isEndDrawer(root, buffer, size, i, cursor));

    begin = beginLine && isBeginDrawer(root, buffer, size, i, cursor);
    end = beginLine &&
           isEndDrawer(root, buffer, size, i, cursor);

    if (begin) {
      Region region{root->getDocument()};
      region.first = i + cursor;
      Drawer *drawer = allocator::allocate<Drawer>();
      drawer->setAttribute("content", region);
      drawers.push(drawer);
    }

    if (end && drawers.size() >= 1) {
      Drawer *drawer = drawers.top();
      drawers.pop();      
      Region *region = drawer->getAttribute<Region>("content");
      region->second = i + cursor + 5;
      ParserResultType type = ParserResultType::FULL;

      populateDrawer(drawer, root, buffer, size, cursor);
      ParserResult<Drawer> result{type, drawer};
      _results.push_back(result);
      i += 5;

      // logs
      log() << "xxxxxxxxx drawer xxxxxxxxx" << std::endl;
      log() << "======= head =======" << std::endl;
      log() << drawer->getAttribute<Region>("head")->string() << std::endl;
      log() << "====================" << std::endl;
      log() << "~~~~~~~ body ~~~~~~~" << std::endl;
      log() << drawer->getAttribute<Region>("body")->string() << std::endl;
      log() << "~~~~~~~~~~~~~~~~~~~~" << std::endl;
      log() << "xxxxxxxxxxxxxxxxxxxxxxxx" << std::endl;
    }

    lastCharacter = buffer[i];
  }
}
void populateDrawer(Drawer *drawer, RootElement *root, const char *buffer,
                    std::size_t size, std::size_t cursor) {
  Region *region = drawer->getAttribute<Region>("content");
  Region headRegion =
      getDrawerName(root, buffer, region->second + 1, region->first, cursor);
  drawer->setAttribute("head", headRegion);
  Region bodyRegion{root->getDocument()};
  bodyRegion.first = headRegion.first + (headRegion.string().size() + 2);
  bodyRegion.second = region->second - 6;
  drawer->setAttribute("head", headRegion);
  drawer->setAttribute("body", bodyRegion);
}
} // namespace orgcpp
