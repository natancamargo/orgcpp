#include "orgcpp/parser-manager.h"
#include "element/root.h"
#include "util/log.h"
#include <algorithm>
#include <format>
#include <regex>
#include <vector>

namespace orgcpp {
void ParserManager::treefyResults(
    RootElement *root, std::vector<ParserResult<Element>> treeFreeResults) {
  std::vector<Element *> treefiedElements;

  std::function<bool(Element *, Element *)> contains = [](Element *left,
                                                          Element *right) {
    Region *leftRegion = left->getAttribute<Region>("content");
    Region *rightRegion = right->getAttribute<Region>("content");
    return leftRegion->first <= rightRegion->first &&
           leftRegion->second >= rightRegion->second;
  };

  std::function<void(Element *, Element *)> treefy = [&](Element *left,
                                                         Element *right) {
    std::string leftHead = getHead(left);
    std::string rightHead = getHead(right);

    bool isLeftParent = contains(left, right);
    bool isRightParent = contains(right, left);

    if (isLeftParent) {
      bool childOfChildren = false;
      for (Element *element : left->getChildren()) {
        if (contains(element, right)) {
          std::string head = getHead(element);
          log() << std::format("searching if \"{}\" is child of \"{}\"",
                               rightHead, head)
                << std::endl;
          treefy(element, right);
          childOfChildren = true;
          break;
        }
        if (contains(right, element)) {
          std::string head = getHead(element);
          log() << std::format("searching if \"{}\" is parent of \"{}\"",
                               rightHead, head)
                << std::endl;
          treefy(element, right);
          break;
        }
      }
      if (!childOfChildren) {
        log() << std::format("\"{}\" is child of \"{}\"", rightHead, leftHead)
              << std::endl;
        left->addChild(right);
      }
    } else {
      if (isRightParent) {
        log() << std::format("\"{}\" is child of \"{}\"", leftHead, rightHead)
              << std::endl;
        if (left->getParent()) {
          std::vector<Element *> &children = left->getParent()->getChildren();
          children.erase(std::find(children.begin(), children.end(), left));
        }
        right->addChild(left);
      }
    }

    return;
  };

  std::function<void(std::vector<Element *> &)> sort =
      [&](std::vector<Element *> &elements) {
        if (elements.size() == 0) {
          return;
        }
        std::sort(elements.begin(), elements.end(),
                  [](Element *left, Element *right) {
                    std::string leftHead = getHead(left);
                    std::string rightHead = getHead(right);
                    log() << std::format("sorting \"{}\" and \"{}\"", leftHead,
                                         rightHead)
                          << std::endl;
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
    if (root->getChildren().empty()) {
      log() << std::format("\"{}\" is root child", head) << std::endl;
      root->addChild(element2);
      continue;
    }
    treefy(root, element2);
  }
  log() << "!!!!!!!!!!!!!!!!!!!!!!" << std::endl;

  log() << "??????? sort ???????" << std::endl;
  sort(root->getChildren());
  log() << "??????????????????????" << std::endl;
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
  _drawerParser.parse(root, buffer, size, cursor);

  // collect results
  collectResults(_headlineParser, results);
  collectResults(_listitemParser, results);
  collectResults(_linkParser, results);
  collectResults(_textParser, results);
  collectResults(_drawerParser, results);

  // root attributes
  root->setAttribute("head", "root");
  Region region{root->getDocument()};
  region.first = 0;
  region.second = size;  
  root->setAttribute("content", region);

  // treefy results into an ast like way
  treefyResults(root, results);
}
std::string getHead(Element *element) {
  std::string head;
  if (element->getAttribute<Region>("head")) {
    head = element->getAttribute<Region>("head")->string();
  } else if (element->getAttribute<std::string>("head")) {
    head = *element->getAttribute<std::string>("head");
  }
  head.erase(std::remove(head.begin(), head.end(), '\n'), head.cend());
  return head;
}
} // namespace orgcpp
