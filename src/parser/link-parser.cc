#include "parser/link-parser.h"
#include "element/root.h"
#include "parser/parser.h"
#include "allocator/allocator.h"
#include "allocator/allocator-impl.h"
#include "util/log.h"
namespace orgcpp {
void LinkParser::parse(RootElement *root, const char *buffer,
                       const std::size_t size, std::size_t cursor) {
  _results.clear();
  char lastCharacter = 0x00;  
  char nextCharacter = 0x00;  
  bool begin = false;
  bool middle = false;
  bool end = false;
  bool lastMiddle = false;
  bool lastEnd = false;
  bool headBegin = false;
  bool headMiddle = false;
  bool headEnd = false;
  bool lastHeadMiddle = false;
  bool lastHeadEnd = false;  
  Region region{root->getDocument()};
  Region headRegion{root->getDocument()};
  ParserResultType resultType = ParserResultType::NONE;
  Link *link;
  for (std::size_t i = 0; i < size; i++) {
    nextCharacter = i < size - 1 ? buffer[i + 1] : 0x00;

    begin = (lastCharacter == '[' && buffer[i] == '[');
    middle = (lastMiddle && !lastEnd) || begin;
    end = (lastMiddle && (lastCharacter == ']' && buffer[i] == ']'));

    headBegin = lastMiddle && lastCharacter == '[';
    headMiddle = (lastHeadMiddle && !lastHeadEnd) || begin;
    headEnd = (lastHeadMiddle && buffer[i] == ']' && nextCharacter == ']'); 

    if (headBegin) {
      headRegion.first = i + cursor;
    }
    
    if (headEnd) {
      headRegion.second = i + cursor;
      link->setAttribute("head", headRegion);
    }

    if (begin) {
      region.first = i + cursor - 1;
      link = allocator::allocate<Link>();
      resultType = ParserResultType::PARTIAL;
    }    

    if (end) {
      region.second = i + cursor + 1;
      resultType = ParserResultType::FULL;

      if (link && link->getAttribute<Region>("head")) {
        link->setAttribute("content", region);
        ParserResult<Link> result{resultType, link};
        _results.push_back(result);

        // logs
        log() << "xxxxxxxxx link xxxxxxxxx" << std::endl;
        log() << "======= head =======" << std::endl;
        log() << link->getAttribute<Region>("head")->string() << std::endl;
        log() << "====================" << std::endl;
        log() << "xxxxxxxxxxxxxxxxxxxxxxxx" << std::endl;
      }
    }
    
    lastMiddle = middle;
    lastEnd = end;
    lastHeadMiddle = headMiddle;    
    lastHeadEnd = headEnd;        
    lastCharacter = buffer[i];    
  }
}
} // namespace orgcpp
