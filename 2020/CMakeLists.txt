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

add_executable(day02 day02.c)
target_link_libraries(day02 PRIVATE common)

add_custom_command(TARGET day02
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy
  ${CMAKE_SOURCE_DIR}/input02.txt
  ${CMAKE_BINARY_DIR}/input02.txt
)

add_executable(day03 day03.c)
target_link_libraries(day03 PRIVATE common)

add_executable(day04 day04.c)
target_link_libraries(day04 PRIVATE common)

add_custom_command(TARGET day04
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy
  ${CMAKE_SOURCE_DIR}/input04.txt
  ${CMAKE_BINARY_DIR}/input04.txt
)

add_executable(day06 day06.c)
target_link_libraries(day06 PRIVATE common)

add_custom_command(TARGET day06
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy
  ${CMAKE_SOURCE_DIR}/input06.txt
  ${CMAKE_BINARY_DIR}/input06.txt
)

add_executable(day07 day07.c)
target_include_directories(day07 SYSTEM PRIVATE stb)
target_link_libraries(day07 PRIVATE common)

add_custom_command(TARGET day07
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy
  ${CMAKE_SOURCE_DIR}/input07.txt
  ${CMAKE_BINARY_DIR}/input07.txt
)
