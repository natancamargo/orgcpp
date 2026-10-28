#pragma once

#include "attribute/attribute.h"
#include <functional>
#include <map>
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
class Element {
public:
  using TraverseFunction = std::function<bool(Element *_element)>;

  virtual ~Element() = default;

  Element *findOneByType(Type type);
  std::vector<Element *> findAllByType(Type type);

  const std::map<std::string, Attribute> &getAttributes() const;
  void setAttribute(const std::string &name, const AttributeValue &value);
  template <typename T> std::optional<T> getAttribute(const std::string &name);

  Element *getParent() const;
  void setParent(Element *parent);

  std::vector<Element*> getChildren() const;

  const Type getType() const;
  void setType(const Type type);

  void traverse(TraverseFunction traverseFunc);
  static void traverse(Element *element,
                       TraverseFunction traverseFunc);

  Element *get(std::size_t index) const;
  template <typename T> T *get(std::size_t index) const;

  void addChild(Element *element);
  void removeChild(Element *element);
  bool isChild(Element *element);

protected:  
  std::map<std::string, Attribute> _attributes;
  Element *_parent;
  std::vector<Element *> _children;
  Type _type;
};
} // namespace orgcpp
