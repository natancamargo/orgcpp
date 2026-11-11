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
protected:
  Type _type = Type::TEXT;
  TextType _textType = TextType::NORMAL;
};
} // namespace orgcpp
