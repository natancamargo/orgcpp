#pragma once

#include "element/property.h"
#include "element/root.h"
#include "parser/parser.h"
namespace orgcpp {
class PropertyParser : public Parser<Property> {
public:
  void parse(RootElement *root, const char *buffer, const std::size_t size,
             std::size_t cursor) override;
};
void populateProperty(Property *property, RootElement *root, const char *buffer,
                      std::size_t size, std::size_t cursor);
} // namespace orgcpp
