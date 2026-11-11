#pragma once

#include <cstddef>
namespace orgcpp {
bool isBeginListitem(const char *buffer, const std::size_t size,
                     const std::size_t i);
bool isBeginHeadline(const char *buffer, const std::size_t size,
                     const std::size_t i);
std::size_t lineIndentation(const char *buffer, const std::size_t size,
                            const std::size_t start);
} // namespace orgcpp
