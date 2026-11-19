#pragma once

#include "element/latex.h"
#include "element/root.h"
#include "parser/parser.h"
namespace orgcpp {
class LatexParser : public Parser<Latex> {
public:
  void parse(RootElement *root, const char *buffer, const std::size_t size,
             std::size_t cursor) override;
};
void populateLatex(Latex *latex, RootElement *root, const char *buffer,
                      std::size_t size, std::size_t cursor);
} // namespace orgcpp
