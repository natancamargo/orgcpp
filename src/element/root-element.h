#pragma once

#include "element/element.h"
#include <fstream>
#include <memory>
namespace orgcpp {
class RootElement : public Element {
public:
  const std::vector<std::string> &getTodos();
  void setTodos(const std::vector<std::string> &todos);

  std::shared_ptr<std::ifstream> getDocument() const;
  void setDocument(std::shared_ptr<std::ifstream> document);

private:
  std::vector<std::string> _todos = {"TODO", "DONE"};
  std::shared_ptr<std::ifstream> _document;
};
} // namespace orgcpp
