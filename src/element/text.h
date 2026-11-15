#pragma once

#include "element/element.h"
namespace orgcpp {
enum class TextType {
  NORMAL,
  BOLD,
  ITALIC,
  VERBATIM,
  CODE
};
class Text : public Element {
public:
  Text();  
protected:
  TextType _textType = TextType::NORMAL;
};
} // namespace orgcpp
