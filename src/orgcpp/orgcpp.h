#pragma once

#include "config/config.h"
#include <cstddef>
#include <string>

#include "attribute/attribute.h"
#include "element/element-impl.h"
#include "element/element.h"
#include "element/headline.h"
#include "element/root-element.h"
#include "matcher/matcher.h"

namespace orgcpp {
enum class Error { NONE, PARSE_FILE_OPEN_ERROR };
extern Error error;
RootElement parse(std::string path);
std::tuple<bool, std::shared_ptr<std::ifstream>>
openFile(const std::string &path);
bool readFileByBlocks(std::shared_ptr<std::ifstream>, std::size_t blockSize,
                      std::function<void(char *buffer)> callback);
std::vector<Element*>
processMatches(std::vector<Match<Element>> unsortedMatches);
} // namespace orgcpp
