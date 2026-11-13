#pragma once

#include "element/root.h"
#include "region/region.h"
#include <cstddef>
#include <string>
namespace orgcpp {
bool isBeginListitem(const char *buffer, const std::size_t size,
                     const std::size_t i);
bool isBeginHeadline(const char *buffer, const std::size_t size,
                     const std::size_t i);
Region getDrawerName(RootElement *root, const char *buffer,
                     const std::size_t size, const std::size_t offset,
                     const std::size_t cursor);
bool isBeginDrawer(RootElement *root, const char *buffer,
                   const std::size_t size, const std::size_t offset,
                   const std::size_t cursor);
bool isEndDrawer(RootElement *root, const char *buffer, const std::size_t size,
                 const std::size_t offset, const std::size_t cursor);
std::size_t lineIndentation(const char *buffer, const std::size_t size,
                            const std::size_t start);
char getCharAfterSpaces(const char *buffer, const std::size_t size,
                     const std::size_t offset);
} // namespace orgcpp
