cmake_minimum_required(VERSION 3.23)
project(bin_tree_printer)

set(CMAKE_CXX_STANDARD 23)

include_directories("./dependency")
include_directories("./include")
add_executable(bin_tree_printer main.cpp include/char_box.h include/tree_printer.h examples/bin_tree_test.cpp examples/tree_test.cpp)
add_definitions(-DFMT_HEADER_ONLY)
