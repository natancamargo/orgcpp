#include "parser/text-parser.h"
#include "allocator/allocator-impl.h"
#include "element/root.h"
#include "parser/parser.h"
#include "util/log.h"
#include "util/parsing.h"
#include <algorithm>
#include <cstddef>
namespace orgcpp {
void TextParser::parse(RootElement *root, const char *buffer,
                       const std::size_t size, std::size_t cursor) {
  _results.clear();
  char lastCharacter = 0x00;
  bool mark = false;
  bool begin = false;
  bool middle = false;
  bool end = false;
  bool lastBegin = false;
  bool lastMiddle = false;
  bool lastEnd = false;
  bool beginFile = false;
  bool endFile = false;
  bool lineBegin = false;
  char charAfterSpaces = 0x00;
  std::size_t indentation = 0;
  std::size_t lastIndentation = 0;
  std::size_t indentationChanged = 0;
  Region region{root->getDocument()};
  ParserResultType resultType = ParserResultType::NONE;
  Text *text;
  for (std::size_t i = 0; i < size; i++) {
    beginFile = i == 0;
    endFile = i == size - 1;
    charAfterSpaces = getCharAfterSpaces(buffer, size, i);
    lineBegin = beginFile || lastCharacter == '\n';
    indentation = lineBegin ? lineIndentation(buffer, size, i) : indentation;
    indentationChanged = (indentation != lastIndentation) && !beginFile;
    mark =
        lineBegin &&
        (isBeginHeadline(buffer, size, i) || isBeginListitem(buffer, size, i) ||
         charAfterSpaces == ':' || charAfterSpaces == '#' ||
         charAfterSpaces == '|' || charAfterSpaces == '\\' ||
         charAfterSpaces == '[' || charAfterSpaces == '<');

    begin =
        (lineBegin && !mark && (!lastMiddle || indentationChanged));
    middle = (lastMiddle && (!lastEnd || lastBegin)) || begin;
    end = lastMiddle && (mark || endFile || indentationChanged);

    if (end) {
      region.second = i + cursor;
      if (endFile) {
        region.second++;
      }
      resultType = ParserResultType::FULL;

      if (text) {
        Region headRegion{region.getDocument()};
        headRegion.first = region.first;
        headRegion.second =
            region.first + ((region.second - region.first) >= 12
                                ? 12
                                : (region.second - region.first));
        text->setAttribute("head", headRegion);
        text->setAttribute("content", region);
        ParserResult<Text> result{resultType, text};
        _results.push_back(result);

        // logs
        log() << "xxxxxxxxx text xxxxxxxxx" << std::endl;
        log() << "======= content =======" << std::endl;
        log() << text->getAttribute<Region>("content")->string() << std::endl;
        log() << "====================" << std::endl;
        log() << "xxxxxxxxxxxxxxxxxxxxxxxx" << std::endl;
      }
    }

    if (begin) {
      region.first = i + cursor;
      text = allocator::allocate<Text>();
      resultType = ParserResultType::PARTIAL;
    }

    if (lineBegin) {
      lastIndentation = indentation;
    }
    lastBegin = begin;
    lastMiddle = middle;
    lastEnd = end;
    lastCharacter = buffer[i];
  }
}
} // namespace orgcpp
