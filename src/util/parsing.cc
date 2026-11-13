#include "util/parsing.h"
#include <cstddef>
#include <iostream>
#include <string>
namespace orgcpp {
bool isBeginListitem(const char *buffer, const std::size_t size,
                     const std::size_t offset) {
  for (std::size_t i = offset; i < size; i++) {
    if (buffer[i] != ' ' && buffer[i] != '-') {
      return false;
    } else if (buffer[i] == '-') {
      return true;
    } else if (buffer[i] == '\n') {
      return false;
    }
  }
  return false;
}
bool isBeginHeadline(const char *buffer, const std::size_t size,
                     const std::size_t offset) {
  for (std::size_t i = offset; i < size; i++) {
    if (buffer[i] != '*') {
      return buffer[i] == ' ' && i != offset;
    }
  }
  return false;
}
Region getDrawerName(RootElement *root, const char *buffer,
                     const std::size_t size, const std::size_t offset,
                     const std::size_t cursor) {
  std::size_t beginIndex = 0;
  char nextCharacter = 0x00;
  bool begin = false, end = false, endBuffer = false;
  Region headRegion{root->getDocument()};
  for (std::size_t i = offset; i < size; i++) {
    endBuffer = i == size - 1;
    nextCharacter = i < size - 1 ? buffer[i + 1] : 0x00;
    if (buffer[i] == '\n') {
      break;
    }

    if (buffer[i] == ' ' && !begin) {
      continue;
    }

    if (buffer[i] != ' ' && buffer[i] != ':' && !begin) {
      break;
    }

    if (buffer[i] == ':' && !begin) {
      begin = true;
      beginIndex = i;
      headRegion.first = i + cursor + 1;
    }

    if (buffer[i] == ' ' && begin) {
      break;
    }

    if (buffer[i] == ':' && begin && i != beginIndex) {
      end = (endBuffer || nextCharacter == '\n');
      if (end) {
        headRegion.second = i + cursor;
      }
      break;
    }
  }
  return headRegion;
}
bool isBeginDrawer(RootElement *root, const char *buffer,
                   const std::size_t size, const std::size_t offset,
                   const std::size_t cursor) {
  Region headRegion = getDrawerName(root, buffer, size, offset, cursor);
  return headRegion.second > 0 && headRegion.string() != "END";
}
bool isEndDrawer(RootElement *root, const char *buffer, const std::size_t size,
                 const std::size_t offset, const std::size_t cursor) {
  Region headRegion = getDrawerName(root, buffer, size, offset, cursor);
  return headRegion.second > 0 && headRegion.string() == "END";
}
std::size_t lineIndentation(const char *buffer, const std::size_t size,
                            const std::size_t offset) {
  std::size_t spaces = 0;
  for (std::size_t i = offset; i < size; i++) {
    if (buffer[i] != ' ') {
      return spaces;
    } else {
      spaces++;
    }
  }
  return spaces;
}
char getCharAfterSpaces(const char *buffer, const std::size_t size,
                     const std::size_t offset) {
  for (std::size_t i = offset; i < size; i++) {
    if (buffer[i] == ' ') {
      continue;
    } else {
      return buffer[i];      
    }
  }
  return 0x00;
}
} // namespace orgcpp
