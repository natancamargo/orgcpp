#include "region/region.h"
#include <fstream>
#include <memory>
#include <string>
namespace orgcpp {
Region::Region(const std::size_t x, const std::size_t y) {
  first = x;
  second = y;
}
std::shared_ptr<std::ifstream> Region::getDocument() const { return _document; }
void Region::setDocument(std::shared_ptr<std::ifstream> document) {
  _document = document;
}

const std::string Region::read() const { return ""; }
} // namespace orgcpp
