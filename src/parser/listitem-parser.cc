#include "parser/listitem-parser.h"
#include "allocator/allocator-impl.h"
#include "parser/parser-impl.h"
#include "element/listitem.h"
#include "element/root.h"
#include "parser/parser.h"
#include "util/log.h"
#include <cstddef>
#include <ostream>
#include <string>
namespace orgcpp {
void ListitemParser::parse(RootElement *root, const char *buffer,
                            const std::size_t size, std::size_t cursor) {
  _results.clear();
  char lastCharacter = 0x00;
  bool lastSpacesBefore = false;
  bool spacesBefore = false;
  bool lastSpacesBeforeHead = false;
  bool spacesBeforeHead = false;
  bool lastSpacesBeforeBody = false;
  bool spacesBeforeBody = false;
  bool lastBegin = false;
  bool begin = false;
  bool lastMiddle = false;
  bool middle = false;
  bool end = false;
  bool headBegin = false;
  bool lastHeadMiddle = false;
  bool headMiddle = false;
  bool lastHeadEnd = false;
  bool headEnd = false;
  bool bodyBegin = false;
  bool lastBodyMiddle = false;
  bool bodyMiddle = false;
  bool bodyEnd = false;
  bool endOfFile = false;
  bool nested = false;
  bool wordBreak = false;
  Region region{root->getDocument()};
  Region headRegion{root->getDocument()};
  Region bodyRegion{root->getDocument()};
  bool resetSpaces = false;
  int lastSpaces = 0;
  int spaces = 0;
  int level = 0;
  int parentLevel = 0;
  std::size_t savedIndex = 0;
  ParserResultType resultType = ParserResultType::NONE;
  Listitem *listitem;
  for (std::size_t i = 0; i < size; i++) {
    endOfFile = i == (size - 1);

    spacesBefore = (lastCharacter == '\n' && buffer[i] == ' ') ||
                   (lastSpacesBefore && buffer[i] == ' ');
    spacesBeforeHead = (lastBegin && buffer[i] == ' ') ||
                       (lastSpacesBeforeHead && buffer[i] == ' ');
    spacesBeforeBody = (lastHeadEnd && buffer[i] == ' ') ||
                       (lastSpacesBeforeBody && buffer[i] == ' ');
    resetSpaces = buffer[i] == '\n';

    level = lastSpaces / 2 + 1;
    wordBreak = ((lastCharacter == '\n' || lastSpacesBefore) &&
                 buffer[i] != ' ' && buffer[i] != '-' && buffer[i] != '\n') &&
                (spaces <= (parentLevel - 1) * 2);

    begin = (i == 0 && buffer[i] == '-') ||
            (lastCharacter == '\n' && buffer[i] == '-') ||
            (lastSpacesBefore && buffer[i] == '-');
    end = (lastMiddle && (begin || endOfFile || wordBreak));
    middle = (lastMiddle && !end) || begin;

    nested = begin && parentLevel > 0 && parentLevel < level;

    headBegin = (lastSpacesBeforeHead && buffer[i] != ' ' && buffer[i] != '\n');
    headEnd = lastHeadMiddle && buffer[i] == '\n';
    headMiddle = (lastHeadMiddle && !headEnd) || headBegin;

    bodyBegin = (lastHeadEnd && !wordBreak && !begin);
    bodyEnd =
        lastBodyMiddle && (begin || endOfFile || wordBreak);
    bodyMiddle = (lastBodyMiddle && !bodyEnd) || bodyBegin;

    if (spacesBefore) {
      spaces++;
    }
    if (resetSpaces) {
      spaces = 0;
    }

    if (nested) {
      begin = false;
      middle = true;
      end = false;
      bodyEnd = false;
      bodyMiddle = true;
      if (savedIndex == 0) {
        savedIndex = i - spaces - 1;
      }
    }

    if (headBegin) {
      headRegion.first = i + cursor;
      listitem->setAttribute("level", level);
      parentLevel = level;
    }
    if (headEnd) {
      headRegion.second = i + cursor;
      listitem->setAttribute("head", headRegion);
    }

    if (bodyBegin) {
      bodyRegion.first = i + cursor;
    }
    if (bodyEnd) {
      bodyRegion.second = i + cursor;
      if (endOfFile) {
        bodyRegion.second++;
      }
      listitem->setAttribute("body", bodyRegion);
    }

    if (end) {
      region.second = i + cursor;
      if (endOfFile) {
        region.second++;
      }
      resultType = ParserResultType::FULL;

      if (listitem && listitem->getAttribute<Region>("head")) {
        listitem->setAttribute("content", region);
        ParserResult<Listitem> result{resultType, listitem};
        _results.push_back(result);

        // logs
        log() << "xxxxxxx listitem xxxxxxx" << std::endl;
        log() << "------- level ------" << std::endl;
        log() << *listitem->getAttribute<int>("level") << std::endl;
        log() << "--------------------" << std::endl;
        log() << "======= head =======" << std::endl;
        log() << listitem->getAttribute<Region>("head")->string() << std::endl;
        log() << "====================" << std::endl;
        if (listitem->getAttribute<Region>("body")) {
          log() << "~~~~~~~ body ~~~~~~~" << std::endl;
          log() << listitem->getAttribute<Region>("body")->string()
                << std::endl;
          log() << "~~~~~~~~~~~~~~~~~~~~" << std::endl;
        }
        log() << "xxxxxxxxxxxxxxxxxxxxxxxx" << std::endl;

        if (savedIndex) {
          i = savedIndex;
          savedIndex = 0;
          parentLevel = 0;
          log() << "^^" << std::endl;
        }
      }
    }

    if (begin) {
      region.first = i + cursor;
      listitem = allocator::allocate<Listitem>();
      resultType = ParserResultType::PARTIAL;
    }

    lastHeadEnd = headEnd;
    lastBodyMiddle = bodyMiddle;
    lastHeadMiddle = headMiddle;
    lastBegin = begin;
    lastMiddle = middle;
    lastSpacesBefore = spacesBefore;
    lastSpacesBeforeHead = spacesBeforeHead;
    lastSpacesBeforeBody = spacesBeforeBody;
    lastSpaces = spaces;
    lastCharacter = buffer[i];
  }
}
} // namespace orgcpp
