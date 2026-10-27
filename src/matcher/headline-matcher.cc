#include "matcher/headline-matcher.h"
#include "allocator/allocator-impl.h"
#include "element/headline.h"
#include "matcher/matcher.h"
#include "util/string.h"
#include <cstddef>
#include <functional>
#include <iostream>
#include <string>

namespace orgcpp {
void HeadlineMatcher::match(std::shared_ptr<std::ifstream> in,
                            const std::string &buffer, std::size_t cursor) {

  std::size_t innerCursor = cursor;
  std::function<bool(char)> isNULL = [](char c) { return c == 0x00; };
  for (std::size_t i = 0; i < buffer.size(); i = (innerCursor - cursor)) {
    bool found = false;
    Region region;
    region.setDocument(in);
    MatchType matchType = MatchType::NONE;
    Headline *headline;
    int numberOfWildcards, numberOfSpaces = 0;
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
      while (j < buffer.size() && (buffer[j] == '*' || buffer[j] == ' ')) {
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
      bool foundHead = validPreHead && buffer[j] != '\n';
      if (!foundHead) {
        innerCursor++;
        continue;        
      }
      if (foundHead) {
        std::cout << "xxxxxxx headline xxxxxxx" << std::endl;
        headline->setAttribute("level", numberOfWildcards);
        Region headRegion;
        headRegion.setDocument(in);
        headRegion.first = ++innerCursor;
        std::cout << "======= head =======" << std::endl;
        while (j < buffer.size() && (buffer[j] != '\n')) {
          std::cout << buffer[j];
          innerCursor++;
          j++;
        }
        headRegion.second = innerCursor;
        headline->setAttribute("head", headRegion);
        matchType = MatchType::FULL;
        std::cout << std::endl << "====================" << std::endl;        
      }
      std::size_t savedInnerCursor = 0;
      bool foundBody = foundHead && buffer[j] == '\n';
      if (foundBody) {
        Region bodyRegion;
        bodyRegion.setDocument(in);
        bodyRegion.first = innerCursor + 1;
        std::cout << "~~~~~~~ body ~~~~~~~";
        while (j < buffer.size()) {
          bool foundAnotherHeadline =
              (j <= (buffer.size() - 1) && (buffer[j] == '\n') &&
               (buffer[j + 1] == '*'));
          std::cout << buffer[j];
          if (foundAnotherHeadline) {
            std::size_t k = j + 1;
            int anotherHeadlineLevel = 0;
            while (k < buffer.size() && buffer[k] == '*') {
              anotherHeadlineLevel++;
              k++;
            }
            if (numberOfWildcards >= anotherHeadlineLevel) {
              innerCursor++;              
              break;
            } else if (!savedInnerCursor) {
              std::cout << "^";
              savedInnerCursor = innerCursor;
            }
          }
          innerCursor++;
          j++;
        }
        bodyRegion.second = innerCursor;
        region.second = innerCursor;
        headline->setAttribute("body", bodyRegion);
                std::cout << std::endl << "~~~~~~~~~~~~~~~~~~~~" << std::endl;        
      }

      if (!foundBody) {
        region.second = innerCursor;
      }

      headline->setAttribute("content", region);

      Match<Headline> match{matchType, headline};
      _matches.push_back(match);

      if (savedInnerCursor) {
        std::cout << "^^" << std::endl;
        innerCursor = savedInnerCursor;
      }
      std::cout << "xxxxxxxxxxxxxxxxxxxxxxxx" << std::endl;
      continue;      
    }
    innerCursor++;
  }
}
} // namespace orgcpp
