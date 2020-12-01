cmake_minimum_required(VERSION 3.13)
project(advent-of-code)

add_library(common INTERFACE)
target_compile_features(common INTERFACE c_std_99)
target_compile_options(common INTERFACE
  -Werror
  -pedantic-errors
  -Wall
  -Wextra
  -Wconversion
  -Wsign-conversion
)

add_executable(day01 day01.c)
target_link_libraries(day01 PRIVATE common)