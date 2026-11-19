#pragma once

#include "element/date.h"
#include "element/root.h"
#include "parser/parser.h"
namespace orgcpp {
class DateParser : public Parser<Date> {
public:
  void parse(RootElement *root, const char *buffer, const std::size_t size,
             std::size_t cursor) override;
};
void populateDate(Date *date, RootElement *root, const char *buffer,
                      std::size_t size, std::size_t cursor);
} // namespace orgcpp
