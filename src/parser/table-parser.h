#pragma once

#include "element/table.h"
#include "element/root.h"
#include "parser/parser.h"
namespace orgcpp {
class TableParser : public Parser<Table> {
public:
  void parse(RootElement *root, const char *buffer, const std::size_t size,
             std::size_t cursor) override;
};
void populateTable(Table *table, RootElement *root, const char *buffer,
                    std::size_t size, std::size_t cursor);
} // namespace orgcpp
