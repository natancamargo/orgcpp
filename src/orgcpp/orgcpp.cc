#include "allocator/allocator.h"
#include "allocator/allocator-impl.h"
#include "orgcpp/orgcpp.h"
#include "config/config.h"
#include "util/string.h"
#include "element/root-element.h"
#include "matcher/headline-matcher.h"
#include "matcher/matcher-impl.h"
#include "matcher/matcher.h"

#include <cstddef>
#include <format>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <tuple>
#include <vector>

namespace orgcpp {
Error error = Error::NONE;
RootElement *parse(std::string path) {
  error = Error::NONE;
  std::cout << "parse: started." << std::endl;
  RootElement *root = allocator::allocate<RootElement>();

  // matchersq
  HeadlineMatcher headlineMatcher;

  // open file
  std::tuple<bool, std::shared_ptr<std::ifstream>> openFileResult =
      openFile(path);
  root->setDocument(std::get<1>(openFileResult));
  if (!std::get<0>(openFileResult)) {
    error = Error::PARSE_FILE_OPEN_ERROR;
    std::cout << "parse: failed." << std::endl;
  }

  // loop file
  std::size_t cursor = 0;
  const int blockSize = config::blockSize;
  bool readResult = readFileByBlocks(
      root->getDocument(), blockSize,
      [&cursor, &headlineMatcher, &root, blockSize](char *buffer) {
        std::string bufferAsString = nullTerminatedCharArrayToString(buffer, blockSize);
        std::vector<Match<Element>> treeFreeMatches;

        // match
        headlineMatcher.match(root->getDocument(), bufferAsString, cursor);
        if (headlineMatcher.getMatches().size() > 0) {
          for (Match<Headline> match : headlineMatcher.getMatches()) {
            if (std::get<0>(match) == MatchType::FULL) {
              treeFreeMatches.push_back(match);
            }
          }
        }

        // process matches into elements
        std::vector<Element *> elements = treefyMatches(treeFreeMatches);

        // populate root
        for (Element *element : elements) {
          root->addChild(element);
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
treefyMatches(std::vector<Match<Element>> treeFreeMatches) {
  std::vector<Element *> treefiedElements;

  std::function<void(Element *&, Element *)> treefy = [&](Element *&left,
                                                          Element *right) {
    Region leftRegion = left->getAttribute<Region>("content").value();
    Region rightRegion = right->getAttribute<Region>("content").value();
    bool isLeftParent = leftRegion.first <= rightRegion.first &&
                        leftRegion.second >= rightRegion.second;
    bool isRightParent = rightRegion.first <= leftRegion.first &&
                         rightRegion.second >= leftRegion.second;

    if (isLeftParent) {
      if (left->getChildren().size() > 0) {
        for (Element *child : left->getChildren()) {
          treefy(child, right);
        }
      } else if (left->getChildren().empty()) {
        left->addChild(right);
      }
    } else {
      if (isRightParent) {
        // TODO
      } else {
        treefiedElements.push_back(right);
      }
      return;
    }

    return;
  };

  for (std::size_t i = 0; i < treeFreeMatches.size(); i++) {
    Element *element2 = std::get<1>(treeFreeMatches[i]);
    if (treefiedElements.empty()) {
      treefiedElements.push_back(element2);
      continue;
    }
    treefy(*treefiedElements.begin(), element2);
  }

  return treefiedElements;
}
} // namespace orgcpp
