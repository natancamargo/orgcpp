#include "orgcpp/parser-manager.h"
#include "util/log.h"
#include <algorithm>
#include <format>
#include <regex>

namespace orgcpp {
std::vector<Element *> ParserManager::treefyResults(
    std::vector<ParserResult<Element>> treeFreeResults) {
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
    std::string leftHead = getHead(left);
    std::string rightHead = getHead(right);;
    bool isLeftParent = contains(left, right);
    bool isRightParent = contains(right, left);

    if (isLeftParent) {
      std::vector<Element *> children = left->getChildren();
      std::vector<Element *>::iterator iterator =
          std::find_if(children.begin(), children.end(), [&](Element *element) {
            return contains(element, right);
          });
      if (iterator != children.end()) {
        std::string head = getHead(*iterator);
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
  for (std::size_t i = 0; i < treeFreeResults.size(); i++) {
    Element *element2 = std::get<1>(treeFreeResults[i]);
    std::string head = getHead(element2);
    log() << std::format("treefying: \"{}\"", head) << std::endl;
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
template <typename T>
void ParserManager::collectResults(
    Parser<T> &parser, std::vector<ParserResult<Element>> &results) {
  for (const ParserResult<Element> match : parser.getResults()) {
    if (std::get<0>(match) == ParserResultType::FULL) {
      results.push_back(match);
    }
  }
}
void ParserManager::parse(RootElement *root, const char *buffer,
                          const std::size_t size, std::size_t cursor) {
  // parsing
  std::vector<ParserResult<Element>> results;
  _headlineParser.parse(root, buffer, size, cursor);
  _listitemParser.parse(root, buffer, size, cursor);
  _linkParser.parse(root, buffer, size, cursor);
  _textParser.parse(root, buffer, size, cursor);

  // collect results
  collectResults(_headlineParser, results);
  collectResults(_listitemParser, results);
  collectResults(_linkParser, results);
  collectResults(_textParser, results);

  // treefy results into an ast like way
  std::vector<Element *> elements = treefyResults(results);
  // populate root
  for (Element *element : elements) {
    root->addChild(element);
  }
}
std::string getHead(Element *element) {
  std::string head = element->getAttribute<Region>("head")->string();
  head.erase(std::remove(head.begin(), head.end(), '\n'), head.cend());
  return head;
}
} // namespace orgcpp
