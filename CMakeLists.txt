cmake_minimum_required(VERSION 3.5)
project(compiler_cmake)

set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++14 -Wall")

find_package(FLEX REQUIRED)

FLEX_TARGET(MyScanner scanner.l  ${CMAKE_CURRENT_SOURCE_DIR}/scanner.cpp COMPILE_FLAGS --header-file=${CMAKE_CURRENT_SOURCE_DIR}/scanner.h)


add_executable(mcc  ${FLEX_MyScanner_OUTPUTS} main.cpp Parser.cpp Parser.h SymbolTable.h)
