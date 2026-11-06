#include "orgcpp/orgcpp.h"
#include "allocator/allocator-impl.h"
#include "allocator/allocator.h"
#include "config/config.h"
#include "element/element.h"
#include "element/listitem.h"
#include "element/root.h"
#include "matcher/headline-matcher.h"
#include "matcher/listitem-matcher.h"
#include "matcher/matcher-impl.h"
#include "matcher/matcher.h"
#include "util/log.h"
#include "util/string.h"

#include <algorithm>
#include <cstddef>
#include <filesystem>
#include <format>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <memory>
#include <ostream>
#include <string>
#include <tuple>
#include <variant>
#include <vector>

namespace orgcpp {
Error error = Error::NONE;
RootElement *parse(std::string path) {
  error = Error::NONE;
  log() << "parse: started." << std::endl;
  log() << std::format("******* {} *******",
                       std::filesystem::path{path}.filename().string())
        << std::endl;
  RootElement *root = allocator::allocate<RootElement>();

  // open file
  std::tuple<bool, std::shared_ptr<std::ifstream>> openFileResult =
      openFile(path);
  root->setDocument(std::get<1>(openFileResult));
  if (!std::get<0>(openFileResult)) {
    error = Error::PARSE_FILE_OPEN_ERROR;
    log() << "parse: failed." << std::endl;
    return root;
  }

  // matchers
  HeadlineMatcher headlineMatcher;
  ListitemMatcher listitemMatcher;

  // read file by blocks
  std::size_t cursor = 0;
  const int blockSize = config::blockSize;
  bool readResult =
      readFileByBlocks(root->getDocument(), blockSize,
                       [&](const char *buffer, const std::size_t size) {
                         std::vector<Match<Element>> matches;

                         // match
                         match(headlineMatcher, listitemMatcher, matches, root,
                               buffer, size, cursor);

                         // treefy matches into an tree of elements
                         std::vector<Element *> elements =
                             treefyMatches(matches);
                         // populate root
                         for (Element *element : elements) {
                           root->addChild(element);
                         }

                         cursor += blockSize;
                       });

  if (!readResult) {
    error = Error::PARSE_FILE_OPEN_ERROR;
    log() << "parse: failed." << std::endl;
  }

  log() << "*********************" << std::endl;
  log() << "parse: done." << std::endl;

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

bool readFileByBlocks(
    std::shared_ptr<std::ifstream> in, std::size_t blockSize,
    std::function<void(const char *buffer, const std::size_t size)> callback) {
  try {
    std::vector<char> buffer(blockSize, 0);
    while (!in->eof()) {
      in->read(buffer.data(), buffer.size());
      // size
      std::size_t size = blockSize;
      std::vector<char>::iterator it =
          std::find(buffer.begin(), buffer.end(), 0x00);
      if (it != buffer.end()) {
        size = std::distance(buffer.begin(), it);
      }
      callback(buffer.data(), size);
    }
  } catch (const std::ifstream::failure &e) {
    log() << std::format("Error: {}\nError code: {}\n", e.what(),
                         e.code().value());
    return false;
  }

  return true;
}
std::vector<Element *>
treefyMatches(std::vector<Match<Element>> treeFreeMatches) {
  std::vector<Element *> treefiedElements;

  std::function<bool(Element *, Element *)> contains = [](Element *left,
                                                          Element *right) {
    Region *leftRegion = left->getAttribute<Region>("content");
    Region *rightRegion = right->getAttribute<Region>("content");
    return leftRegion->first <= rightRegion->first &&
           leftRegion->second >= rightRegion->second;
  };

  std::function<void(Element *&, Element *)> treefy = [&](Element *&left,
                                                          Element *right) {
    std::string leftHead = left->getAttribute<Region>("head")->string();
    std::string rightHead = right->getAttribute<Region>("head")->string();
    bool isLeftParent = contains(left, right);
    bool isRightParent = contains(right, left);

    if (isLeftParent) {
      std::vector<Element *> children = left->getChildren();
      std::vector<Element *>::iterator iterator =
          std::find_if(children.begin(), children.end(), [&](Element *element) {
            return contains(element, right);
          });
      if (iterator != children.end()) {
        std::string head = (*iterator)->getAttribute<Region>("head")->string();
        log() << std::format("searching if \"{}\" is child of \"{}\"",
                             rightHead, head)
              << std::endl;
        treefy(*iterator, right);
      } else {
        log() << std::format("\"{}\" is child of \"{}\"", rightHead, leftHead)
              << std::endl;
        left->addChild(right);
      }
    } else {
      if (isRightParent) {
        log() << std::format("\"{}\" is child of \"{}\"", leftHead, rightHead)
              << std::endl;
        log() << "Not implemented (right is parent)" << std::endl;
        // TODO
      }
      return;
    }

    return;
  };

  std::function<void(std::vector<Element *> &)> sort =
      [&](std::vector<Element *> &elements) {
        if (elements.empty()) {
          return;
        }
        std::sort(elements.begin(), elements.end(),
                  [](Element *left, Element *right) {
                    Region *leftRegion = left->getAttribute<Region>("content");
                    Region *rightRegion =
                        right->getAttribute<Region>("content");
                    return leftRegion->first < rightRegion->first;
                  });
        for (Element *element : elements) {
          sort(element->getChildren());
        }
      };

  log() << "!!!!!!! treefy !!!!!!!" << std::endl;
  for (std::size_t i = 0; i < treeFreeMatches.size(); i++) {
    Element *element2 = std::get<1>(treeFreeMatches[i]);
    std::string head = element2->getAttribute<Region>("head")->string();
    log() << "treefying: " << head << std::endl;
    if (treefiedElements.empty()) {
      log() << element2->getAttribute<Region>("head")->string() << " is at root"
            << std::endl;
      treefiedElements.push_back(element2);
      continue;
    }
    std::vector<Element *>::iterator iterator = std::find_if(
        treefiedElements.begin(), treefiedElements.end(),
        [&](Element *element1) { return contains(element1, element2); });
    if (iterator != treefiedElements.end()) {
      treefy(*iterator, element2);
    } else {
      log() << std::format("\"{}\" is root child", head) << std::endl;
      treefiedElements.push_back(element2);
    }
  }
  log() << "!!!!!!!!!!!!!!!!!!!!!!" << std::endl;

  sort(treefiedElements);

  return treefiedElements;
}
void match(HeadlineMatcher &headlineMatcher, ListitemMatcher &listitemMatcher,
           std::vector<Match<Element>> &matches, RootElement *root,
           const char *buffer, const std::size_t size, std::size_t cursor) {
  headlineMatcher.match(root, buffer, size, cursor);
  if (headlineMatcher.getMatches().size() > 0) {
    for (const Match<Element> match : headlineMatcher.getMatches()) {
      if (std::get<0>(match) == MatchType::FULL) {
        matches.push_back(match);
      }
    }
  }
  listitemMatcher.match(root, buffer, size, cursor);
  if (listitemMatcher.getMatches().size() > 0) {
    for (const Match<Element> match : listitemMatcher.getMatches()) {
      if (std::get<0>(match) == MatchType::FULL) {
        matches.push_back(match);
      }
    }
  }
}
} // namespace orgcpp
