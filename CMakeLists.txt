cmake_minimum_required (VERSION 3.1)

project (lazyTA)

set (CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "-Wall")

file(GLOB SRCS
  "src/*.cpp"
  # For QT Creator
  "include/*.h"
)
include_directories(
  include
)
add_executable(lazyTA ${SRCS})
