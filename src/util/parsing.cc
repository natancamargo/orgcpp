#include "util/parsing.h"
#include <cstddef>
namespace orgcpp {
bool isBeginListitem(const char *buffer, const std::size_t size,
                     const std::size_t start) {
  for (std::size_t i = start; i < size; i++) {
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
                     const std::size_t start) {
  for (std::size_t i = start; i < size; i++) {
    if (buffer[i] != '*') {
      return buffer[i] == ' ' && i != start;
    }
  }    
  return false;
}
std::size_t lineIndentation(const char *buffer, const std::size_t size,
                        const std::size_t start) {
  std::size_t spaces = 0;  
  for (std::size_t i = start; i < size; i++) {
    if (buffer[i] != ' ') {
      return spaces;
    } else {
      spaces++;      
    }      
  }    
  return spaces;  
}
}
