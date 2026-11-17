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
Region getBlockBeginName(RootElement *root, const char *buffer,
                         const std::size_t size, const std::size_t offset,
                         const std::size_t cursor);
Region getBlockEndName(RootElement *root, const char *buffer,
                       const std::size_t size, const std::size_t offset,
                       const std::size_t cursor);
Region getBlockType(RootElement *root, const char *buffer,
                    const std::size_t size, const std::size_t offset,
                    const std::size_t cursor);
bool isBeginBlock(RootElement *root, const char *buffer, const std::size_t size,
                  const std::size_t offset, const std::size_t cursor);
bool isEndBlock(RootElement *root, const char *buffer, const std::size_t size,
                const std::size_t offset, const std::size_t cursor);
Region getKeywordName(RootElement *root, const char *buffer,
                      const std::size_t size, const std::size_t offset,
                      const std::size_t cursor);
Region getKeywordValue(RootElement *root, const char *buffer,
                      const std::size_t size, const std::size_t offset,
                      const std::size_t cursor);
bool isBeginKeyword(RootElement *root, const char *buffer,
                    const std::size_t size, const std::size_t offset,
                    const std::size_t cursor);
std::size_t getLineBreakIndex(const char *buffer, const std::size_t size,
                              const std::size_t offset);
std::size_t getLineBreakReversedIndex(const char *buffer,
                                      const std::size_t size,
                                      const std::size_t offset);
} // namespace orgcpp
