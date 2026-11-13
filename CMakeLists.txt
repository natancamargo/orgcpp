cmake_minimum_required(VERSION 3.5.0)
project(orgcpp VERSION 0.0.1)

include(FetchContent)

# generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# c standard
# ----------------------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD          20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

# compile options
# ----------------------------------------------------------------------------------------
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  add_compile_options(
    -Wall
    -Wextra
    -Werror
    -Wno-unused-parameter
  )
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  add_compile_options(/W4)
endif()

# dependencies
# ----------------------------------------------------------------------------------------
# vcpkg
include($ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)

find_package(Catch2 CONFIG REQUIRED)

# target library
# ----------------------------------------------------------------------------------------
set(LIBRARY_NAME orgcpp)
add_library(${LIBRARY_NAME} STATIC
  "${CMAKE_CURRENT_SOURCE_DIR}/src/attribute/attribute.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/attribute/checkbox.cc"

  "${CMAKE_CURRENT_SOURCE_DIR}/src/orgcpp/orgcpp.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/orgcpp/parser-manager.cc"

  "${CMAKE_CURRENT_SOURCE_DIR}/src/element/element.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/element/listitem.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/element/root.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/element/headline.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/element/drawer.cc"

  "${CMAKE_CURRENT_SOURCE_DIR}/src/parser/headline-parser.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/parser/listitem-parser.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/parser/link-parser.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/parser/text-parser.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/parser/drawer-parser.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/parser/parser.cc"

  "${CMAKE_CURRENT_SOURCE_DIR}/src/region/region.cc"

  "${CMAKE_CURRENT_SOURCE_DIR}/src/util/parsing.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/util/string.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/util/log.cc"
)
target_link_libraries(${LIBRARY_NAME} PUBLIC)
target_include_directories(${LIBRARY_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")

# target executable
# ----------------------------------------------------------------------------------------
set(EXECUTABLE_NAME orgcpp-cli)
add_executable(${EXECUTABLE_NAME}
  "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cc"
)
target_link_libraries(${EXECUTABLE_NAME} PUBLIC ${LIBRARY_NAME} Catch2::Catch2WithMain)
set_target_properties(${EXECUTABLE_NAME} PROPERTIES
RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME})
set_property(TARGET ${EXECUTABLE_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:${PROJECT_NAME}>)

# target tests
# ----------------------------------------------------------------------------------------
set(TEST_NAME "orgcpp-unit-tests")

add_executable(${TEST_NAME}
  "${CMAKE_CURRENT_SOURCE_DIR}/tests/headline.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/tests/listitem.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/tests/link.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/tests/text.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/tests/drawer.cc"
)
target_link_libraries(${TEST_NAME} PUBLIC ${LIBRARY_NAME} Catch2::Catch2WithMain)
set_target_properties(${TEST_NAME} PROPERTIES
RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME})

# config file
# ----------------------------------------------------------------------------------------
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/src/config/config.h.in"
  "${CMAKE_CURRENT_BINARY_DIR}/config/config.h" ESCAPE_QUOTES
)
target_include_directories(${LIBRARY_NAME} PUBLIC "${CMAKE_CURRENT_BINARY_DIR}")

# copy resources
# ----------------------------------------------------------------------------------------
add_custom_command(
  TARGET ${TEST_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/tests/resources ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/resources
)
