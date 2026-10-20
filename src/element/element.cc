#include "element/element.h"
#include "element/elements.h"

namespace orgcpp {
Element::Element() { _children = new Elements(); }
Element::~Element() { delete _children; }

Element *Element::findOneByType(Type type) {
  Element *element;
  traverse([&element, &type](Element *_element) {
    if (element) {
      return false;
    } else if (type == _element->getType()) {
      element = _element;
      return false;
    }
    return true;
  });
  return element;
}
Elements *Element::findAllByType(Type type) {
  Elements *elements;
  traverse([&elements, &type](Element *_element) {
    if (type == _element->getType()) {
      elements->addChild(_element);      
      return false;
    }
    return true;
  });
  return elements;  
}

const std::string Element::getHead() const {
  return _readableHeadRegion.read(_document);
}
void Element::setHead(const std::string &head) { _writetableHead = head; }

const std::string Element::getBody() const {
  return _readableBodyRegion.read(_document);
}
void Element::setBody(const std::string &body) { _writetableBody = body; }

std::ifstream *Element::getDocument() const { return _document; }

Element *Element::getParent() const { return _parent; }

Elements *Element::getChildren() const { return _children; }

const Type Element::getType() const { return _type; }
void Element::setType(const Type type) { _type = type; }

void Element::traverse(TraverseFunction traverseFunc) {
  traverse(this, traverseFunc);
}
void Element::traverse(Element *element, TraverseFunction traverseFunc) {
  Elements *elements = element->getChildren();
  for (Element *_element : elements->getElements()) {
    if (!traverseFunc(_element)) {
      return;
    }
  }
}

} // namespace orgcpp
