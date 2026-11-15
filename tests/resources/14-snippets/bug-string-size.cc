#include <regex>
#include <string>
#include <format>
#include <iostream> 

int main() {
  std::regex regex;
  regex = "(^|\n)?(\\*+)(\\s+)([^\n]+)(\n([\\s\\S]+))?";
  std::string test = R"(* Headline
Text
)";
  test.resize(64 * 1024);
  std::cout << test;

  std::sregex_iterator sRegexBegin =
      std::sregex_iterator(test.begin(), test.end(), regex);
  std::sregex_iterator sRegexEnd = std::sregex_iterator();
  std::size_t matchCount = std::distance(sRegexBegin, sRegexEnd);
  std::cout << std::format("match count: {}\n", matchCount);
  for (std::sregex_iterator i = sRegexBegin; i != sRegexEnd; ++i) {
    std::cout << std::format("sub match count: {}\n", i->size());  
  }
}
