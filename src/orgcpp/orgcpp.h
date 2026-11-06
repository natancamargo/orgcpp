#pragma once

#include "config/config.h"
#include <cstddef>
#include <string>

#include "attribute/attribute.h"
#include "element/element-impl.h"
#include "element/element.h"
#include "element/headline.h"
#include "element/listitem.h"
#include "element/root.h"
#include "matcher/headline-matcher.h"
#include "matcher/listitem-matcher.h"
#include "matcher/matcher.h"

namespace orgcpp {
enum class Error { NONE, PARSE_FILE_OPEN_ERROR };
extern Error error;
RootElement *parse(std::string path);
std::tuple<bool, std::shared_ptr<std::ifstream>>
openFile(const std::string &path);
bool readFileByBlocks(std::shared_ptr<std::ifstream> in, std::size_t blockSize,
                      std::function<void(const char *buffer, const std::size_t size)> callback);
std::vector<Element *>
treefyMatches(std::vector<Match<Element>> treeFreeMatches);
void match(HeadlineMatcher &headlineMatcher, ListitemMatcher &listitemMatcher,
           std::vector<Match<Element>> &matches, RootElement *root,
           const char *buffer, const std::size_t size, std::size_t cursor);
} // namespace orgcpp
