#include "orgcpp/orgcpp.h"
#include "config/config.h"
#include "element/root-element.h"
#include "matcher/headline-matcher.h"
#include "matcher/matcher-impl.h"
#include "matcher/matcher.h"

#include <cstddef>
#include <format>
#include <fstream>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <tuple>
#include <vector>

namespace orgcpp {
Error error = Error::NONE;
RootElement parse(std::string path) {
  error = Error::NONE;
  std::cout << "parse: started." << std::endl;
  RootElement root;

  // matchersq
  HeadlineMatcher headlineMatcher;

  // open file
  std::tuple<bool, std::shared_ptr<std::ifstream>> openFileResult =
      openFile(path);
  root.setDocument(std::get<1>(openFileResult));
  if (!std::get<0>(openFileResult)) {
    error = Error::PARSE_FILE_OPEN_ERROR;
    std::cout << "parse: failed." << std::endl;
  }

  // loop file
  std::size_t cursor = 0;
  const int blockSize = config::blockSize;
  bool readResult = readFileByBlocks(
      root.getDocument(), blockSize,
      [&cursor, &headlineMatcher, &root, blockSize](char *buffer) {
        std::string bufferAsString = std::string{buffer, buffer + blockSize};
        std::vector<Match<Element>> unsortedMatches;

        // match
        headlineMatcher.match(root.getDocument(), bufferAsString, cursor);
        if (headlineMatcher.getMatches().size() > 0) {
          for (Match<Headline> match : headlineMatcher.getMatches()) {
            if (std::get<0>(match) == MatchType::FULL) {
              unsortedMatches.push_back(match);
            }
          }
        }

        // process matches into elements
        std::vector<Element*> elements = processMatches(unsortedMatches);

        // populate root
        for (Element *element : elements) {
          root.addChild(element);
        }

        cursor += blockSize;
      });

  if (!readResult) {
    error = Error::PARSE_FILE_OPEN_ERROR;
    std::cout << "parse: failed." << std::endl;
  }

  std::cout << "parse: done." << std::endl;

  return root;
}
std::tuple<bool, std::shared_ptr<std::ifstream>>
openFile(const std::string &path) {
  std::shared_ptr<std::ifstream> in = std::make_shared<std::ifstream>(path);
  if (!in->is_open()) {
    perror("Error while opening the file.\n");
    return std::make_tuple(false, in);
  }

  in->exceptions(in->badbit);

  return std::make_tuple(true, in);
}

bool readFileByBlocks(std::shared_ptr<std::ifstream> in, std::size_t blockSize,
                      std::function<void(char *buffer)> callback) {
  try {
    std::vector<char> buffer(blockSize, 0);
    while (!in->eof()) {
      in->read(buffer.data(), buffer.size());
      callback(buffer.data());
    }
  } catch (const std::ifstream::failure &e) {
    std::cout << std::format("Error: {}\nError code: {}\n", e.what(),
                             e.code().value());
    return false;
  }

  return true;
}
std::vector<Element *>
processMatches(std::vector<Match<Element>> unsortedMatches) {
  std::vector<Element *> sortedElements;
  for (Match<Element> match : unsortedMatches) {
    Element *element = std::get<2>(match);
    sortedElements.push_back(element);
  }
  return sortedElements;
}
} // namespace orgcpp
