#pragma once

#include "region/region.h"
#include <fstream>
#include <functional>
#include <map>
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
enum class Mode { WRITE, READ };
class Elements;
class Element {
public:
  using TraverseFunction = std::function<bool(Element *_element)>;

  Element();
  ~Element();

  Element *findOneByType(Type type);
  Elements *findAllByType(Type type);

  const std::map<std::string, Region> &getReadableAttributes() const;
  const std::map<std::string, std::string> &getWritableAttributes() const;
  void setReadableAttribute(std::string key, Region value);
  void setWritableAttribute(std::string key, std::string value);

  std::ifstream *getDocument() const;

  Element *getParent() const;

  Elements *getChildren() const;

  const Type getType() const;
  void setType(const Type type);

  const Mode getMode() const;
  void setMode(const Mode mode);

  void traverse(TraverseFunction traverseFunc);
  static void traverse(Element *element, TraverseFunction traverseFunc);

private:
  std::ifstream *_document;
  std::map<std::string, Region> _readableAttributes;
  std::map<std::string, std::string> _writableAttributes;
  Element *_parent;
  Elements *_children;
  Type _type;
  Mode _mode;
};
} // namespace orgcpp
