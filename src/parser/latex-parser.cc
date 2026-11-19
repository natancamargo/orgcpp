#include "parser/latex-parser.h"
#include "allocator/allocator-impl.h"
#include "allocator/allocator.h"
#include "element/root.h"
#include "parser/latex-parser.h"
#include "parser/parser.h"
#include "util/log.h"
#include "util/parsing.h"
#include <cstddef>
#include <stack>
namespace orgcpp {
void LatexParser::parse(RootElement *root, const char *buffer,
                        const std::size_t size, std::size_t cursor) {
  _results.clear();
  char lastCharacter = 0x00;
  bool begin = false;
  bool end = false;
  bool beginLine = false;
  bool beginFile = false;
  std::map<std::string, std::stack<Latex *>> latexes;
  for (std::size_t i = 0; i < size; i++) {
    beginFile = i == 0;
    beginLine = lastCharacter == '\n' || beginFile;

    begin = beginLine && isBeginLatex(root, buffer, size, i, cursor);
    end = beginLine && isEndLatex(root, buffer, size, i, cursor);

    if (begin) {
      Region region{root->getDocument()};
      region.first = i + cursor;
      Latex *latex = allocator::allocate<Latex>();
      latex->setAttribute("content", region);
      std::string latexName =
          getLatexBeginName(root, buffer, size, i, cursor).string();
      latexes[latexName].push(latex);
    }

    if (end) {
      std::string latexName =
          getLatexEndName(root, buffer, size, i, cursor).string();
      std::stack latexStack = latexes[latexName];
      if (latexStack.size() >= 1) {
        Latex *latex = latexStack.top();
        latexStack.pop();
        Region *region = latex->getAttribute<Region>("content");
        region->second = i + cursor;
        ParserResultType type = ParserResultType::FULL;

        populateLatex(latex, root, buffer, size, cursor);
        ParserResult<Latex> result{type, latex};
        _results.push_back(result);
        
        // logs
        log() << "xxxxxxxxx latex xxxxxxxxx" << std::endl;
        log() << "======= head =======" << std::endl;
        log() << latex->getAttribute<Region>("head")->string() << std::endl;
        log() << "====================" << std::endl;
        if (latex->getAttribute<Region>("latexType")) {
          log() << "******* type *******" << std::endl;
          log() << latex->getAttribute<Region>("latexType")->string()
                << std::endl;
          log() << "********************" << std::endl;
        }
        log() << "~~~~~~~ body ~~~~~~~" << std::endl;
        log() << latex->getAttribute<Region>("body")->string() << std::endl;
        log() << "~~~~~~~~~~~~~~~~~~~~" << std::endl;
        log() << "xxxxxxxxxxxxxxxxxxxxxxxx" << std::endl;
      }
    }

    lastCharacter = buffer[i];
  }
}
void populateLatex(Latex *latex, RootElement *root, const char *buffer,
                   std::size_t size, std::size_t cursor) {
  Region *region = latex->getAttribute<Region>("content");
  Region headRegion = getLatexBeginName(root, buffer, region->second + 1,
                                        region->first, cursor);
  latex->setAttribute("head", headRegion);
  Region bodyRegion{root->getDocument()};
  bodyRegion.first = getLineBreakIndex(buffer, size, region->first) + 1;
  bodyRegion.second = region->second - 1;
  latex->setAttribute("body", bodyRegion);
  Region endRegion =
      getLatexEndName(root, buffer, size, region->second, cursor);
  region->second = endRegion.second; // update content
}
} // namespace orgcpp
