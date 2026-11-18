#pragma once

#include "element/tag.h"
#include "element/root.h"
#include "parser/parser.h"
namespace orgcpp {
class TagParser : public Parser<Tag> {
public:
  void parse(RootElement *root, const char *buffer, const std::size_t size,
             std::size_t cursor) override;
};
void populateTag(Tag *tag, RootElement *root, const char *buffer,
                      std::size_t size, std::size_t cursor);
} // namespace orgcpp
