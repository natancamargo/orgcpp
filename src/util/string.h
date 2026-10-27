#pragma once

#include <cstddef>
#include <string>
namespace orgcpp {
std::string nullTerminatedCharArrayToString(char *buffer, std::size_t size);
void printChar(const char* buffer, std::size_t);
}
