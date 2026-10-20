#pragma once

#include "region/region.h"
#include <fstream>
#include <functional>
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
  
  Element();
  ~Element();
  
  Element *findOneByType(Type type);
  Elements *findAllByType(Type type);

  const std::string getHead() const;
  void setHead(const std::string &head);

  const std::string getBody() const;
  void setBody(const std::string &body);

  std::ifstream *getDocument() const;

  Element *getParent() const;

  Elements *getChildren() const;

  const Type getType() const;
  void setType(const Type body);
  
  void traverse(TraverseFunction traverseFunc);
  static void traverse(Element *element, TraverseFunction traverseFunc);

private:
  std::ifstream *_document;
  Region _readableHeadRegion;
  Region _readableBodyRegion;
  std::string _writetableHead;
  std::string _writetableBody;
  Element *_parent;
  Elements *_children;
  Type _type;
};
} // namespace orgcpp
