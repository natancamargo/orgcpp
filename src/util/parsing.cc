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
std::size_t getIndexAfterSpaces(const char *buffer, const std::size_t size,
                                const std::size_t offset) {
  for (std::size_t i = offset; i < size; i++) {
    if (buffer[i] == ' ') {
      continue;
    } else {
      return i;
    }
  }
  return 0x00;
}
Region getBlockBeginName(RootElement *root, const char *buffer,
                         const std::size_t size, const std::size_t offset,
                         const std::size_t cursor) {
  Region region{root->getDocument()};
  std::string pattern = "#+begin_";
  std::size_t matchCount = 0;
  bool beginName = false;
  bool endName = false;
  for (std::size_t i = offset; i < size; i++) {
    if (buffer[i] == ' ' && matchCount == 0) {
      continue;
    }
    if (buffer[i] == '\n') {
      break;
    }
    if (matchCount >= pattern.size()) {
      if (!beginName) {
        region.first = i + cursor;
        beginName = true;
      }
      if (buffer[i] == ' ' && !endName) {
        region.second = i + cursor;
        endName = true;
      }
      if ((i < size - 1 && buffer[i + 1] == '\n') && !endName) {
        region.second = i + cursor + 1;
        endName = true;
      }
    } else {
      if (buffer[i] == pattern[matchCount]) {
        matchCount++;
      } else {
        break;
      }
    }
  }
  return region;
}
Region getBlockEndName(RootElement *root, const char *buffer,
                       const std::size_t size, const std::size_t offset,
                       const std::size_t cursor) {
  Region region{root->getDocument()};
  std::string pattern = "#+end_";
  std::size_t matchCount = 0;
  bool beginName = false;
  for (std::size_t i = offset; i < size; i++) {
    if (buffer[i] == ' ' && matchCount == 0) {
      continue;
    }
    if (buffer[i] == '\n') {
      break;
    }
    if (matchCount >= pattern.size()) {
      if (!beginName) {
        region.first = i + cursor;
        beginName = true;
      }
      if (i == size - 1 || buffer[i + 1] == '\n') {
        region.second = i + cursor + 1;
      }
    } else {
      if (buffer[i] == pattern[matchCount]) {
        matchCount++;
      } else {
        break;
      }
    }
  }
  return region;
}
Region getBlockType(RootElement *root, const char *buffer,
                    const std::size_t size, const std::size_t offset,
                    const std::size_t cursor) {
  Region region{root->getDocument()};
  bool beginName = false;
  for (std::size_t i = offset; i < size; i++) {
    if (buffer[i] == ' ') {
      continue;
    }
    if (buffer[i] != ' ' && !beginName) {
      region.first = i + cursor;
      beginName = true;
    }
    if (buffer[i] == '\n' || i == size - 1) {
      region.second = i + cursor;
      break;
    }
  }
  return region;
}
bool isBeginBlock(RootElement *root, const char *buffer, const std::size_t size,
                  const std::size_t offset, const std::size_t cursor) {
  Region region = getBlockBeginName(root, buffer, size, offset, cursor);
  return region.second > 0;
};
bool isEndBlock(RootElement *root, const char *buffer, const std::size_t size,
                const std::size_t offset, const std::size_t cursor) {
  Region region = getBlockEndName(root, buffer, size, offset, cursor);
  return region.second > 0;
};
std::size_t getLineBreakIndex(const char *buffer, const std::size_t size,
                              const std::size_t offset) {
  for (std::size_t i = offset; i < size; i++) {
    if (buffer[i] == '\n') {
      return i;
    }
  }
  return 0;
}
std::size_t getLineBreakReversedIndex(const char *buffer,
                                      const std::size_t size,
                                      const std::size_t offset) {
  for (std::size_t i = offset; i != 0; i--) {
    if (buffer[i] == '\n') {
      return i;
    }
  }
  return size - 1;
}
Region getKeywordName(RootElement *root, const char *buffer,
                      const std::size_t size, const std::size_t offset,
                      const std::size_t cursor) {
  Region region{root->getDocument()};
  std::string pattern = "#+";
  std::size_t matchCount = 0;
  bool beginName = false;
  bool endName = false;
  for (std::size_t i = offset; i < size; i++) {
    if (buffer[i] == ' ' && matchCount == 0) {
      continue;
    }
    if (buffer[i] == '\n') {
      break;
    }
    if (matchCount >= pattern.size()) {
      if (!beginName) {
        region.first = i + cursor;
        beginName = true;
      }
      if (buffer[i] == ':' && !endName) {
        region.second = i + cursor;
        endName = true;
        break;
      }
    } else {
      if (buffer[i] == pattern[matchCount]) {
        matchCount++;
      } else {
        break;
      }
    }
  }
  return region;
}
Region getKeywordValue(RootElement *root, const char *buffer,
                       const std::size_t size, const std::size_t offset,
                       const std::size_t cursor) {
  Region region{root->getDocument()};
  bool beginName = false;
  for (std::size_t i = offset; i < size; i++) {
    if (buffer[i] == ' ') {
      continue;
    }
    if (buffer[i] != ' ' && !beginName) {
      region.first = i + cursor;
      beginName = true;
    }
    if (buffer[i] == '\n') {
      region.second = i + cursor;
      break;
    }
  }
  return region;
}
bool isBeginKeyword(RootElement *root, const char *buffer,
                    const std::size_t size, const std::size_t offset,
                    const std::size_t cursor) {
  Region region = getKeywordName(root, buffer, size, offset, cursor);
  return region.valid();
}
bool isBeginTable(const char *buffer, const std::size_t size,
                  const std::size_t offset) {
  return getCharAfterSpaces(buffer, size, offset) == '|';
}
bool isEndTable(const char *buffer, const std::size_t size,
                const std::size_t offset) {
  return getCharAfterSpaces(buffer, size, offset) != '|' ||
         isFinalLine(buffer, size, offset);
}
bool isFinalLine(const char *buffer, const std::size_t size,
                 const std::size_t offset) {
  std::size_t index = getLineBreakIndex(buffer, size, offset);
  return index == 0 || index == size - 1;
}
Region getPropertyName(RootElement *root, const char *buffer,
                       const std::size_t size, const std::size_t offset,
                       const std::size_t cursor) {
  std::size_t beginIndex = 0;
  char nextCharacter = 0x00;
  bool begin = false;
  bool endOfFile = false;
  Region headRegion{root->getDocument()};
  for (std::size_t i = offset; i < size; i++) {
    nextCharacter = i < size - 1 ? buffer[i + 1] : 0x00;
    endOfFile = i == size - 1;
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
    if (buffer[i] == ':' && begin && i != beginIndex && nextCharacter != '\n' &&
        !endOfFile) {
      headRegion.second = i + cursor;
      break;
    }
  }
  return headRegion;
}
Region getPropertyValue(RootElement *root, const char *buffer,
                        const std::size_t size, const std::size_t offset,
                        const std::size_t cursor) {
  Region region{root->getDocument()};
  bool beginName = false;
  for (std::size_t i = offset; i < size; i++) {
    if (buffer[i] == ' ') {
      continue;
    }
    if (buffer[i] != ' ' && !beginName) {
      region.first = i + cursor;
      beginName = true;
    }
    if (buffer[i] == '\n') {
      region.second = i + cursor;
      break;
    }
  }
  return region;
}
bool isBeginProperty(RootElement *root, const char *buffer,
                     const std::size_t size, const std::size_t offset,
                     const std::size_t cursor) {
  Region region = getPropertyName(root, buffer, size, offset, cursor);
  return region.valid();
}
Region getTagName(RootElement *root, const char *buffer, const std::size_t size,
                  const std::size_t offset, const std::size_t cursor) {
  std::size_t beginIndex = 0;
  bool begin = false;
  Region region{root->getDocument()};
  for (std::size_t i = offset; i < size; i++) {
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
      region.first = i + cursor + 1;
    }
    if (buffer[i] == ' ' && begin) {
      break;
    }
    if (buffer[i] == ':' && begin && i != beginIndex) {
      region.second = i + cursor;
      break;
    }
  }
  return region;
}
bool isBeginTag(RootElement *root, const char *buffer, const std::size_t size,
                const std::size_t offset, const std::size_t cursor) {
  Region region = getTagName(root, buffer, size, offset, cursor);
  return region.valid();
}
Region getDateValue(RootElement *root, const char *buffer, const std::size_t size,
                  const std::size_t offset, const std::size_t cursor) {
  bool begin = false;
  Region region{root->getDocument()};
  for (std::size_t i = offset; i < size; i++) {
    if (buffer[i] == '\n') {
      break;
    }
    if (buffer[i] == '<' && !begin) {
      begin = true;
      region.first = i + cursor + 1;      
    }
    if (buffer[i] == '>' && begin) {
      region.second = i + cursor;      
      break;
    }
  }
  return region;
}
Region getDatePre(RootElement *root, const char *buffer, const std::size_t size,
                  const std::size_t offset, const std::size_t cursor) {
  Region region{root->getDocument()};
  for (std::size_t i = offset; i < size; i++) {    
    if (buffer[i] == '\n' || buffer[i] == '<') {
      break;
    }
    if (i == offset) {
      region.first = i + cursor;      
    }
    if (buffer[i] == ':') {
      region.second = i + cursor;      
      break;
    }
  }
  return region;
}
bool isBeginDate(RootElement *root, const char *buffer, const std::size_t size,
                  const std::size_t offset, const std::size_t cursor) {
  bool begin = false, beginDate = false;
  Region region{root->getDocument()};
  for (std::size_t i = offset; i < size; i++) {
    if (buffer[i] == '\n') {
      break;
    }
    if (buffer[i] == '<' && !begin) {
      begin = true;
    }
    if (buffer[i] == '>' && begin) {
      beginDate = true;      
      break;
    }
  }
  return beginDate;
}
} // namespace orgcpp
