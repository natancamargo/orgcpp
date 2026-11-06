#include "attribute/attribute.h"

namespace orgcpp {
Attribute::Attribute() : _value("") {}

const std::string &Attribute::getName() const { return _name; }
void Attribute::setName(const std::string &name) { _name = name; }

AttributeValue &Attribute::getValue() { return _value; }
void Attribute::setValue(const AttributeValue &value) { _value = value; };
} // namespace orgcpp
