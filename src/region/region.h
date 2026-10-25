#pragma once

#include <fstream>
#include <memory>
#include <string>
#include <utility>
namespace orgcpp {
class Region : public std::pair<std::size_t, std::size_t> {
public:
  Region() = default;
  Region(const std::size_t x, const std::size_t y);
  const std::string read() const;
  const std::string string() const;

  std::shared_ptr<std::ifstream> getDocument() const;
  void setDocument(std::shared_ptr<std::ifstream> document);
private:
  std::shared_ptr<std::ifstream> _document;
};
} // namespace orgcpp
