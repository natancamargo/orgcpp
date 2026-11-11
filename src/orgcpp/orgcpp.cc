#include "orgcpp/orgcpp.h"
#include "allocator/allocator-impl.h"
#include "config/config.h"
#include "element/root.h"
#include "orgcpp/parser-manager.h"
#include "util/log.h"

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

  // parsers
  ParserManager parserManager;

  // read file by blocks
  std::size_t cursor = 0;
  const int blockSize = config::blockSize;
  bool readResult =
      readFileByBlocks(root->getDocument(), blockSize,
                       [&](const char *buffer, const std::size_t size) {
                         parserManager.parse(root, buffer, size, cursor);
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
    perror("Error opening the file.\n");
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
    std::cerr << std::format("Error: {}\nError code: {}\n", e.what(),
                         e.code().value());
    return false;
  }

  return true;
}
} // namespace orgcpp
