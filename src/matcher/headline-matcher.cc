#include "matcher/headline-matcher.h"
#include "allocator/allocator-impl.h"
#include "element/headline.h"
#include "matcher/matcher.h"
#include <cstddef>
#include <functional>
#include <iostream>
#include <string>

namespace orgcpp {
void HeadlineMatcher::match(std::shared_ptr<std::ifstream> in,
                            const std::string &buffer, std::size_t cursor) {

  std::size_t innerCursor = cursor;
  bool found = false;
  std::size_t numberOfWildcards, numberOfSpaces = 0;
  Region region;
  region.setDocument(in);
  MatchType matchType = MatchType::NONE;
  Headline *headline;
  std::function<bool(char)> isNULL = [](char c) { return c == 0x00; };
  for (std::size_t i = 0; i < buffer.size(); i = (innerCursor - cursor)) {
    if (isNULL(buffer[i])) {
      break;
    }
    if (buffer[i] == '*') {
      bool atBegin = i == 0;
      bool afterLF = i > 0 && buffer[i - 1] == '\n';
      if (atBegin || afterLF) {
        found = true;
        numberOfWildcards = 1;
        numberOfSpaces = 0;        
        matchType = MatchType::PARTIAL;
        region.first = innerCursor;
        headline = allocator::allocate<Headline>();
        headline->setDocument(in);
      }
    }
    if (found) {
      std::size_t j = i + 1;
      while (j < buffer.size() && !isNULL(buffer[j]) &&
             (buffer[j] == '*' || buffer[j] == ' ')) {
        if (buffer[j] == '*') {
          numberOfWildcards++;
        }
        if (buffer[j] == ' ') {
          numberOfSpaces++;          
        }          
        innerCursor++;
        j++;
      }
      bool validPreHead = numberOfSpaces > 0 && numberOfWildcards > 0;
      bool foundHead = validPreHead && !isNULL(buffer[j]) && buffer[j] != '\n';      
      if (foundHead) {
        headline->setAttribute("level", static_cast<int>(numberOfWildcards));
        Region headRegion;
        headRegion.setDocument(in);
        headRegion.first = ++innerCursor;
        while (!isNULL(buffer[j]) && j < buffer.size() && (buffer[j] != '\n')) {
          innerCursor++;
          j++;
        }
        headRegion.second = innerCursor;
        headline->setAttribute("head", headRegion);
        matchType = MatchType::FULL;
      }
      bool foundBody = validPreHead && !isNULL(buffer[j]) && buffer[j] == '\n';            
      if (foundBody) {
        std::size_t k = j + 1;
        Region bodyRegion;
        bodyRegion.setDocument(in);
        bodyRegion.first = ++innerCursor;
        while (!isNULL(buffer[k]) && (k < buffer.size() - 2) &&
               !((buffer[k + 1] == '\n') && (buffer[k + 2] == '*'))) {
          innerCursor++;
          k++;
        }
        if (isNULL(buffer[k]) || k == buffer.size()) {
          bodyRegion.second = innerCursor - 1;
        } else {
          bodyRegion.second = ++innerCursor;
        }
        headline->setAttribute("body", bodyRegion);
      }

      region.second = innerCursor;
      Match<Headline> match{matchType, region, headline};
      _matches.push_back(match);
      found = false;
    }
    innerCursor++;
  }
}
} // namespace orgcpp
