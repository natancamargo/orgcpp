#pragma once

#include "attribute/attribute.h"
#include "region/region.h"
#include <fstream>
#include <functional>
#include <map>
#include <memory>
#include <optional>
namespace orgcpp {
enum class Type {
  HEADLINE,
  DRAWER,
  BLOCK,
  TABLE,
  LINK,
  LIST_ITEM,
  PROPERTIES,
  TAGS,
  DATES_TIMESTAMP,
  DATES_SCHEDULED,
  TEXT,
  LATEX,
  FOOTNOTE
};
class Elements;
class Element {
public:
  using TraverseFunction = std::function<bool(Element *_element)>;

  virtual ~Element() = default;

  Element *findOneByType(Type type);
  Elements *findAllByType(Type type);

  std::shared_ptr<std::ifstream> getDocument() const;
  void setDocument(std::shared_ptr<std::ifstream> document);
  
  const std::map<std::string, Attribute> &getAttributes() const;
  void setAttribute(const std::string& name, const AttributeValue &value);
  template <typename T> std::optional<T> getAttribute(const std::string& name);

  Element *getParent() const;

  Elements *getChildren() const;

  const Type getType() const;
  void setType(const Type type);

  void traverse(TraverseFunction traverseFunc);
  static void traverse(Element *element, TraverseFunction traverseFunc);  
protected:
  std::shared_ptr<std::ifstream> _document;
  std::map<std::string, Attribute> _attributes;
  Element *_parent;
  Elements *_children;
  Type _type;
};
} // namespace orgcpp
