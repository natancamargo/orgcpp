#include "element/element.h"
#include "attribute/attribute.h"
#include <algorithm>
#include <memory>

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
std::vector<Element *> Element::findAllByType(Type type) {
  std::vector<Element *> elements;
  traverse([&elements, &type](Element *_element) {
    if (type == _element->getType()) {
      elements.push_back(_element);
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
void Element::setAttribute(const std::string &name,
                           const AttributeValue &value) {
  Attribute attribute;
  attribute.setName(name);
  attribute.setValue(value);
  _attributes[name] = attribute;
}

Element *Element::getParent() const { return _parent; }
void Element::setParent(Element *parent) { _parent = parent; }

std::vector<Element *> Element::getChildren() const { return _children; }

const Type Element::getType() const { return _type; }
void Element::setType(const Type type) { _type = type; }

void Element::traverse(TraverseFunction traverseFunc) {
  traverse(this, traverseFunc);
}
void Element::traverse(Element *element, TraverseFunction traverseFunc) {
  std::vector<Element *> elements = element->getChildren();
  for (Element *_element : elements) {
    if (!traverseFunc(_element)) {
      return;
    }
  }
}

Element *Element::get(std::size_t index) const {
  try {
    return _children.at(index);
  } catch (const std::out_of_range &e) {
    return nullptr;
  }
}

void Element::addChild(Element *element) {
  _children.push_back(element);
  element->setParent(element);
}

void Element::removeChild(Element *element) {
  element->setParent(nullptr);
  _children.erase(std::remove(_children.begin(), _children.end(), element));
}

bool Element::isChild(Element *element) {
  return std::find(_children.begin(), _children.end(), element) !=
         _children.end();
}

} // namespace orgcpp
