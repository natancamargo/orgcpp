#include "element/root-element.h"

namespace orgcpp {
const std::vector<std::string> &RootElement::getTodos() { return _todos; }
void RootElement::setTodos(const std::vector<std::string> &todos) {
  _todos = todos;
}
void setTodos(const std::vector<std::string> &todos);

std::shared_ptr<std::ifstream> RootElement::getDocument() const {
  return _document;
}
void RootElement::setDocument(std::shared_ptr<std::ifstream> document) {
  _document = document;
}
} // namespace orgcpp
