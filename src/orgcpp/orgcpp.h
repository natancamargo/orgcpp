#pragma once

#include "config/config.h"
#include <cstddef>
#include <string>

#include "attribute/attribute.h"
#include "element/element-impl.h"
#include "element/element.h"
#include "element/headline.h"
#include "element/listitem.h"
#include "element/link.h"
#include "element/text.h"
#include "element/drawer.h"
#include "element/block.h"
#include "element/keyword.h"
#include "element/table.h"
#include "element/property.h"
#include "element/tag.h"
#include "element/date.h"
#include "element/latex.h"
#include "element/footnote.h"
#include "element/root.h"
#include "parser/headline-parser.h"
#include "parser/listitem-parser.h"
#include "parser/link-parser.h"
#include "parser/text-parser.h"
#include "parser/drawer-parser.h"
#include "parser/block-parser.h"
#include "parser/keyword-parser.h"
#include "parser/table-parser.h"
#include "parser/property-parser.h"
#include "parser/tag-parser.h"
#include "parser/date-parser.h"
#include "parser/latex-parser.h"
#include "parser/footnote-parser.h"
#include "parser/parser.h"

namespace orgcpp {
extern bool initialized;
enum class Error { NONE, PARSE_FILE_OPEN_ERROR };
extern Error error;
RootElement *parse(std::string path);
std::tuple<bool, std::shared_ptr<std::ifstream>>
openFile(const std::string &path);
bool readFileByBlocks(std::shared_ptr<std::ifstream> in, std::size_t blockSize,
                      std::function<void(const char *buffer, const std::size_t size)> callback);
} // namespace orgcpp
