#pragma once

#include "element/drawer.h"
#include "element/root.h"
#include "parser/parser.h"
namespace orgcpp {
class DrawerParser : public Parser<Drawer> {
public:
  void parse(RootElement *root, const char *buffer, const std::size_t size,
             std::size_t cursor) override;
};
void populateDrawer(Drawer *drawer, RootElement *root, const char *buffer,
                    std::size_t size, std::size_t cursor);
} // namespace orgcpp
