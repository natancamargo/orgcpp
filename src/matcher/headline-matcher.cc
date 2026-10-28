#include "matcher/headline-matcher.h"
#include "allocator/allocator-impl.h"
#include "config/config.h"
#include "element/headline.h"
#include "element/root-element.h"
#include "matcher/matcher.h"
#include "util/log.h"
#include "util/string.h"
#include <cstddef>
#include <format>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <string>

namespace orgcpp {
void HeadlineMatcher::match(RootElement *root, const std::string &buffer,
                            std::size_t cursor) {
  std::size_t innerCursor = cursor;
  std::shared_ptr<std::ifstream> in = root->getDocument();
  std::function<bool(char)> isNULL = [](char c) { return c == 0x00; };
  for (std::size_t i = 0; i < buffer.size(); i = (innerCursor - cursor)) {
    bool found = false;
    Region region;
    region.setDocument(in);
    MatchType matchType = MatchType::NONE;
    Headline *headline = nullptr;
    int numberOfWildcards = 0, numberOfSpaces = 0;
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
        Region headRegion;
        headRegion.setDocument(in);
        headRegion.first = ++innerCursor;
        while (j < buffer.size() && (buffer[j] != '\n')) {
          innerCursor++;
          j++;
        }
        headRegion.second = innerCursor;
        headline->setAttribute("level", numberOfWildcards);        
        headline->setAttribute("head", headRegion);
        matchType = MatchType::FULL;
        // todo
        const std::string &headlineAsString = headRegion.string();
        for (const std::string &todo : root->getTodos()) {
          if (headlineAsString.find(todo) != std::string::npos) {
            headRegion.first += todo.size() + 1;            
            headline->setAttribute("todo", todo);
            headline->setAttribute("head", headRegion);
            break;
          }
        }
        log() << "xxxxxxx headline xxxxxxx" << std::endl;        
        log() << "======= level =======" << std::endl;
        log() << numberOfWildcards << std::endl;
        log() << "====================" << std::endl;
        log() << "======= head =======" << std::endl;
        log() << headlineAsString;        
        log() << "====================" << std::endl;        
      }
      std::size_t savedInnerCursor = 0;
      bool foundBody = foundHead && buffer[j] == '\n';
      if (foundBody) {
        Region bodyRegion;
        bodyRegion.setDocument(in);
        bodyRegion.first = innerCursor + 1;
        log() << "~~~~~~~ body ~~~~~~~";
        while (j < buffer.size()) {
          bool foundAnotherHeadline =
              (j < (buffer.size() - 1) && (buffer[j] == '\n') &&
               (buffer[j + 1] == '*'));
          log() << buffer[j];
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
              log() << "^";
              savedInnerCursor = innerCursor;
            }
          }
          innerCursor++;
          j++;
        }
        bodyRegion.second = innerCursor;
        region.second = innerCursor;
        headline->setAttribute("body", bodyRegion);
        log() << std::endl << "~~~~~~~~~~~~~~~~~~~~" << std::endl;
      }

      if (!foundBody) {
        region.second = innerCursor;
      }

      headline->setAttribute("content", region);

      Match<Headline> match{matchType, headline};
      _matches.push_back(match);

      if (savedInnerCursor) {
        log() << "^^" << std::endl;
        innerCursor = savedInnerCursor;
      }
      log() << "xxxxxxxxxxxxxxxxxxxxxxxx" << std::endl;
      log() << std::format("> innerCursor: {}", innerCursor) << std::endl;
      continue;
    }
    innerCursor++;
  }
}
} // namespace orgcpp
