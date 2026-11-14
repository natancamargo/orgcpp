#include "config/config.h"
#include <iostream>
#include <ostream>
namespace orgcpp {
std::ostream nullStream{nullptr};
std::ostream coutStream{std::cout.rdbuf()};
std::ostream &log() {
  if (config::debug) {
    return coutStream;
  }
  return nullStream;
};
std::ostream &logSort() {
  if (config::debugSort) {
    return coutStream;
  }
  return nullStream;
};
} // namespace orgcpp
