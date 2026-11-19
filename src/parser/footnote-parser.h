#pragma once

#include "element/footnote.h"
#include "element/root.h"
#include "parser/parser.h"
namespace orgcpp {
class FootnoteParser : public Parser<Footnote> {
public:
  void parse(RootElement *root, const char *buffer, const std::size_t size,
             std::size_t cursor) override;
};
void populateFootnote(Footnote *footnote, RootElement *root, const char *buffer,
                      std::size_t size, std::size_t cursor);
} // namespace orgcpp
