#include "region/region.h"
#include "util/log.h"
#include <fstream>
#include <iostream>
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

const std::string Region::read() const {
  std::ios::iostate oldState = _document->rdstate();
  std::ifstream::pos_type oldPosition = _document->tellg();

  _document->clear();  
  _document->seekg(first);
  std::string content;
  content.resize(second - first);
  _document->read(content.data(), second - first);

  _document->seekg(oldPosition);
  _document->setstate(oldState);  
  return content;
}
const std::string Region::string() const { return read(); }
} // namespace orgcpp
