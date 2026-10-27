#include <algorithm>
#include <cstddef>
#include <string>
namespace orgcpp {
std::string nullTerminatedCharArrayToString(char *buffer, std::size_t size) {
  char *end = buffer + size;
  char *match = std::find(buffer, buffer + size, 0x00);
  if (match != end) {
    return std::string{buffer, match};
  }
  return std::string{buffer, buffer + size};
}
} // namespace orgcpp
