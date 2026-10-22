#include "element/element.h"
#include "attribute/attribute.h"
#include "element/elements.h"

namespace orgcpp {
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

std::shared_ptr<std::ifstream> Element::getDocument() const {
  return _document;
}
void Element::setDocument(std::shared_ptr<std::ifstream> document) {
  _document = document;  
}

const std::map<std::string, Attribute> &Element::getAttributes() const {
  return _attributes;
}
void Element::setAttribute(const std::string &name, const AttributeValue &value) {
  Attribute attribute;
  attribute.setName(name);
  attribute.setValue(value);
  _attributes[name] = attribute;
}

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
